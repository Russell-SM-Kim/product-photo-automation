#include "CameraManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QThread>

CameraManager::CameraManager(
    QObject *parent
)
    : QObject(parent),
      watcher(
          new QFileSystemWatcher(this)
      ),
      scanTimer(
          new QTimer(this)
      )
{
    scanTimer->setSingleShot(
        true
    );

    scanTimer->setInterval(
        500
    );

    connect(
        watcher,
        &QFileSystemWatcher::directoryChanged,
        this,
        &CameraManager::handleDirectoryChanged
    );

    connect(
        scanTimer,
        &QTimer::timeout,
        this,
        &CameraManager::scanIncomingFolder
    );
}

void CameraManager::setIncomingFolder(
    const QString &folderPath
)
{
    bool wasWatching =
        isWatching();

    if (wasWatching)
    {
        stopWatching();
    }

    incomingFolder =
        QDir::cleanPath(
            folderPath
        );

    if (wasWatching)
    {
        startWatching();
    }
}

void CameraManager::setProductsFolder(
    const QString &folderPath
)
{
    productsFolder =
        QDir::cleanPath(
            folderPath
        );
}

QString CameraManager::getIncomingFolder() const
{
    return incomingFolder;
}

QString CameraManager::getProductsFolder() const
{
    return productsFolder;
}

void CameraManager::startWatching()
{
    if (incomingFolder.isEmpty())
    {
        emit errorOccurred(
            "카메라 수신 폴더가 지정되지 않았습니다."
        );

        return;
    }

    if (productsFolder.isEmpty())
    {
        emit errorOccurred(
            "상품 사진 저장 폴더가 지정되지 않았습니다."
        );

        return;
    }

    if (
        !QDir(
            incomingFolder
        ).exists()
    )
    {
        if (
            !QDir().mkpath(
                incomingFolder
            )
        )
        {
            emit errorOccurred(
                QString(
                    "Incoming 폴더 생성 실패 : %1"
                )
                    .arg(
                        incomingFolder
                    )
            );

            return;
        }
    }

    if (
        !QDir(
            productsFolder
        ).exists()
    )
    {
        if (
            !QDir().mkpath(
                productsFolder
            )
        )
        {
            emit errorOccurred(
                QString(
                    "Products 폴더 생성 실패 : %1"
                )
                    .arg(
                        productsFolder
                    )
            );

            return;
        }
    }

    initializeKnownFiles();

    if (
        !watcher
             ->directories()
             .contains(
                 incomingFolder
             )
    )
    {
        bool success =
            watcher->addPath(
                incomingFolder
            );

        if (!success)
        {
            emit errorOccurred(
                QString(
                    "폴더 감시 시작 실패 : %1"
                )
                    .arg(
                        incomingFolder
                    )
            );

            return;
        }
    }

    emit watchingStarted(
        incomingFolder
    );
}

void CameraManager::stopWatching()
{
    scanTimer->stop();

    const QStringList watchedFolders =
        watcher->directories();

    for (
        const QString &folder :
        watchedFolders
    )
    {
        watcher->removePath(
            folder
        );
    }

    emit watchingStopped();
}

void CameraManager::setCurrentBarcode(
    const QString &barcode
)
{
    activeBarcode =
        barcode;
}

void CameraManager::clearCurrentBarcode()
{
    activeBarcode.clear();
}

bool CameraManager::isWatching() const
{
    return !watcher
                ->directories()
                .isEmpty();
}

void CameraManager::handleDirectoryChanged(
    const QString &path
)
{
    Q_UNUSED(path);

    scanTimer->start(
        500
    );
}

void CameraManager::scanIncomingFolder()
{
    if (incomingFolder.isEmpty())
    {
        return;
    }

    QDir dir(
        incomingFolder
    );

    QFileInfoList files =
        dir.entryInfoList(
            QDir::Files |
            QDir::NoDotAndDotDot,
            QDir::Time
        );

    for (
        const QFileInfo &fileInfo :
        files
    )
    {
        QString fileName =
            fileInfo.fileName();

        if (
            !isImageFile(
                fileName
            )
        )
        {
            continue;
        }

        if (
            knownFiles.contains(
                fileName
            )
        )
        {
            continue;
        }

        QString fullPath =
            fileInfo
                .absoluteFilePath();

        if (
            !isFileReady(
                fullPath
            )
        )
        {
            scanTimer->start(
                500
            );

            continue;
        }

        emit photoDetected(
            fullPath
        );

        processPhoto(
            fullPath
        );

        knownFiles.append(
            fileName
        );
    }
}

void CameraManager::initializeKnownFiles()
{
    knownFiles.clear();

    if (incomingFolder.isEmpty())
    {
        return;
    }

    QDir dir(
        incomingFolder
    );

    QFileInfoList files =
        dir.entryInfoList(
            QDir::Files |
            QDir::NoDotAndDotDot
        );

    for (
        const QFileInfo &fileInfo :
        files
    )
    {
        knownFiles.append(
            fileInfo.fileName()
        );
    }
}

bool CameraManager::isImageFile(
    const QString &fileName
) const
{
    QString extension =
        QFileInfo(
            fileName
        )
            .suffix()
            .toLower();

    return (
        extension == "jpg" ||
        extension == "jpeg" ||
        extension == "png" ||
        extension == "arw"
    );
}

bool CameraManager::isFileReady(
    const QString &filePath
) const
{
    QFileInfo firstInfo(
        filePath
    );

    if (
        !firstInfo.exists() ||
        firstInfo.size() <= 0
    )
    {
        return false;
    }

    qint64 firstSize =
        firstInfo.size();

    QThread::msleep(
        150
    );

    QFileInfo secondInfo(
        filePath
    );

    if (
        !secondInfo.exists()
    )
    {
        return false;
    }

    return (
        firstSize ==
            secondInfo.size() &&
        secondInfo.size() > 0
    );
}

void CameraManager::processPhoto(
    const QString &filePath
)
{
    if (
        activeBarcode.isEmpty()
    )
    {
        emit errorOccurred(
            QString(
                "사진은 감지됐지만 현재 바코드가 없습니다 : %1"
            )
                .arg(
                    filePath
                )
        );

        return;
    }

    QFileInfo sourceInfo(
        filePath
    );

    QString extension =
        sourceInfo
            .suffix()
            .toLower();

    QString productFolder =
        createProductFolder(
            activeBarcode
        );

    if (
        productFolder.isEmpty()
    )
    {
        return;
    }

    int nextIndex =
        findNextPhotoIndex(
            productFolder,
            activeBarcode
        );

    QString newFileName =
        QString(
            "%1_%2.%3"
        )
            .arg(
                activeBarcode
            )
            .arg(
                nextIndex
            )
            .arg(
                extension
            );

    QString destinationPath =
        QDir(
            productFolder
        ).filePath(
            newFileName
        );

    bool moved =
        QFile::rename(
            filePath,
            destinationPath
        );

    if (!moved)
    {
        bool copied =
            QFile::copy(
                filePath,
                destinationPath
            );

        if (!copied)
        {
            emit errorOccurred(
                QString(
                    "사진 저장 실패 : %1"
                )
                    .arg(
                        filePath
                    )
            );

            return;
        }

        QFile::remove(
            filePath
        );
    }

    emit photoSaved(
        activeBarcode,
        destinationPath,
        nextIndex
    );
}

QString CameraManager::createProductFolder(
    const QString &barcode
)
{
    QString folderPath =
        QDir(
            productsFolder
        )
            .filePath(
                barcode
            );

    if (
        !QDir(
            folderPath
        ).exists()
    )
    {
        if (
            !QDir().mkpath(
                folderPath
            )
        )
        {
            emit errorOccurred(
                QString(
                    "상품 폴더 생성 실패 : %1"
                )
                    .arg(
                        folderPath
                    )
            );

            return QString();
        }
    }

    return folderPath;
}

int CameraManager::findNextPhotoIndex(
    const QString &productFolder,
    const QString &barcode
) const
{
    QDir dir(
        productFolder
    );

    QStringList filters;

    filters
        << QString(
               "%1_*.*"
           )
               .arg(
                   barcode
               );

    QFileInfoList files =
        dir.entryInfoList(
            filters,
            QDir::Files
        );

    int maxIndex =
        0;

    QRegularExpression regex(
        QString(
            "^%1_(\\d+)\\."
        )
            .arg(
                QRegularExpression::escape(
                    barcode
                )
            ),
        QRegularExpression::
            CaseInsensitiveOption
    );

    for (
        const QFileInfo &fileInfo :
        files
    )
    {
        QRegularExpressionMatch match =
            regex.match(
                fileInfo.fileName()
            );

        if (!match.hasMatch())
        {
            continue;
        }

        bool ok =
            false;

        int number =
            match
                .captured(1)
                .toInt(
                    &ok
                );

        if (
            ok &&
            number > maxIndex
        )
        {
            maxIndex =
                number;
        }
    }

    return maxIndex + 1;
}