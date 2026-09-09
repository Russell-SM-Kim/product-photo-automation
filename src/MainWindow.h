#pragma once

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QComboBox;

class ScaleManager;
class CameraManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
        QWidget *parent = nullptr
    );

private:
    // =========================================
    // 설정 UI
    // =========================================

    QComboBox *serialPortCombo;

    QPushButton *refreshPortsButton;

    QLineEdit *incomingFolderEdit;
    QPushButton *browseIncomingButton;

    QLineEdit *workspaceFolderEdit;
    QPushButton *browseWorkspaceButton;

    // =========================================
    // 작업 UI
    // =========================================

    QLabel *barcodeValueLabel;

    QLabel *scaleStatusLabel;
    QLabel *weightValueLabel;

    QLineEdit *barcodeInput;

    QTextEdit *logBox;

    QPushButton *startButton;
    QPushButton *finishProductButton;
    QPushButton *exitButton;

    // =========================================
    // 현재 상품
    // =========================================

    QString currentBarcode;

    double currentWeight =
        0.0;

    bool hasWeight =
        false;

    // =========================================
    // Manager
    // =========================================

    ScaleManager *scaleManager;
    CameraManager *cameraManager;

    // =========================================
    // 함수
    // =========================================

    void handleBarcode();

    void refreshSerialPorts();

    void selectIncomingFolder();

    void selectWorkspaceFolder();

    void loadSettings();

    void saveSettings();

    void applyFolders();

    void writeLog(
        const QString &message
    );
};