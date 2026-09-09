#include "FileManager.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QStringConverter>

QString FileManager::dataFolder;

void FileManager::setDataFolder(
    const QString &folderPath
)
{
    dataFolder =
        QDir::cleanPath(
            folderPath
        );
}

QString FileManager::getDataFolder()
{
    return dataFolder;
}

QString FileManager::getDataFilePath()
{
    if (dataFolder.isEmpty())
    {
        return QString();
    }

    QDir dir;

    if (!dir.exists(dataFolder))
    {
        dir.mkpath(
            dataFolder
        );
    }

    return QDir(
        dataFolder
    ).filePath(
        "product_data.csv"
    );
}

bool FileManager::saveProductData(
    const QString &barcode,
    double weight,
    QString *errorMessage
)
{
    QString filePath =
        getDataFilePath();

    if (filePath.isEmpty())
    {
        if (errorMessage)
        {
            *errorMessage =
                "데이터 저장 폴더가 지정되지 않았습니다.";
        }

        return false;
    }

    QFile file(
        filePath
    );

    bool newFile =
        !file.exists();

    if (
        !file.open(
            QIODevice::WriteOnly |
            QIODevice::Append |
            QIODevice::Text
        )
    )
    {
        if (errorMessage)
        {
            *errorMessage =
                file.errorString();
        }

        return false;
    }

    QTextStream stream(
        &file
    );

    stream.setEncoding(
        QStringConverter::Utf8
    );

    if (newFile)
    {
        stream
            << "timestamp,"
            << "barcode,"
            << "weight_kg"
            << "\n";
    }

    QString timestamp =
        QDateTime::currentDateTime()
            .toString(
                "yyyy-MM-dd HH:mm:ss"
            );

    stream
        << timestamp
        << ","
        << barcode
        << ","
        << QString::number(
               weight,
               'f',
               2
           )
        << "\n";

    file.close();

    return true;
}