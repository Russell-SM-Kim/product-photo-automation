#pragma once

#include <QString>

class FileManager
{
public:
    static void setDataFolder(
        const QString &folderPath
    );

    static QString getDataFolder();

    static bool saveProductData(
        const QString &barcode,
        double weight,
        QString *errorMessage = nullptr
    );

private:
    static QString dataFolder;

    static QString getDataFilePath();
};