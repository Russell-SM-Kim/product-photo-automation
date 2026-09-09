#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QTimer>

class CameraManager : public QObject
{
    Q_OBJECT

public:
    explicit CameraManager(
        QObject *parent = nullptr
    );

    void setIncomingFolder(
        const QString &folderPath
    );

    void setProductsFolder(
        const QString &folderPath
    );

    QString getIncomingFolder() const;
    QString getProductsFolder() const;

    void startWatching();
    void stopWatching();

    void setCurrentBarcode(
        const QString &barcode
    );

    void clearCurrentBarcode();

    bool isWatching() const;

signals:
    void watchingStarted(
        const QString &folderPath
    );

    void watchingStopped();

    void photoDetected(
        const QString &sourcePath
    );

    void photoSaved(
        const QString &barcode,
        const QString &savedPath,
        int photoIndex
    );

    void errorOccurred(
        const QString &message
    );

private slots:
    void handleDirectoryChanged(
        const QString &path
    );

    void scanIncomingFolder();

private:
    QFileSystemWatcher *watcher;
    QTimer *scanTimer;

    QString incomingFolder;
    QString productsFolder;

    QString activeBarcode;

    QStringList knownFiles;

    bool isImageFile(
        const QString &fileName
    ) const;

    bool isFileReady(
        const QString &filePath
    ) const;

    void initializeKnownFiles();

    void processPhoto(
        const QString &filePath
    );

    QString createProductFolder(
        const QString &barcode
    );

    int findNextPhotoIndex(
        const QString &productFolder,
        const QString &barcode
    ) const;
};