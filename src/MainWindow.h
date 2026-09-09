#pragma once

#include <QMainWindow>
#include <QString>

class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QComboBox;
class QFrame;
class QVBoxLayout;
class QResizeEvent;

class ScaleManager;
class CameraManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
        QWidget *parent = nullptr
    );

protected:
    void resizeEvent(
        QResizeEvent *event
    ) override;

private:
    // ====================================================
    // Header
    // ====================================================

    QLabel *titleLabel = nullptr;
    QLabel *subTitleLabel = nullptr;
    QLabel *systemStatusLabel = nullptr;

    // ====================================================
    // 환경 설정
    // ====================================================

    QFrame *settingsCard = nullptr;
    QVBoxLayout *settingsCardLayout = nullptr;

    QLabel *settingsTitle = nullptr;
    QLabel *settingsHint = nullptr;

    QLabel *scalePortTitle = nullptr;
    QLabel *incomingTitle = nullptr;
    QLabel *workspaceTitle = nullptr;
    QLabel *workspaceHint = nullptr;

    QComboBox *serialPortCombo = nullptr;
    QPushButton *refreshPortsButton = nullptr;

    QLineEdit *incomingFolderEdit = nullptr;
    QPushButton *browseIncomingButton = nullptr;

    QLineEdit *workspaceFolderEdit = nullptr;
    QPushButton *browseWorkspaceButton = nullptr;

    // ====================================================
    // 장치 상태
    // ====================================================

    QFrame *deviceCard = nullptr;
    QVBoxLayout *deviceCardLayout = nullptr;

    QLabel *deviceTitle = nullptr;

    QLabel *scaleDotLabel = nullptr;
    QLabel *scaleDeviceNameLabel = nullptr;
    QLabel *scaleStatusLabel = nullptr;

    QLabel *cameraDotLabel = nullptr;
    QLabel *cameraDeviceNameLabel = nullptr;
    QLabel *cameraStatusLabel = nullptr;

    // ====================================================
    // 현재 상품
    // ====================================================

    QFrame *productCard = nullptr;
    QVBoxLayout *productCardLayout = nullptr;

    QLabel *productTitle = nullptr;
    QLabel *activeBadgeLabel = nullptr;

    QLabel *barcodeCaptionLabel = nullptr;
    QLabel *barcodeValueLabel = nullptr;

    QFrame *weightBox = nullptr;
    QFrame *photoBox = nullptr;

    QLabel *weightCaptionLabel = nullptr;
    QLabel *weightValueLabel = nullptr;

    QLabel *photoCaptionLabel = nullptr;
    QLabel *photoCountLabel = nullptr;

    // ====================================================
    // Barcode
    // ====================================================

    QFrame *barcodeCard = nullptr;
    QVBoxLayout *barcodeCardLayout = nullptr;

    QLabel *barcodeInputTitle = nullptr;
    QLabel *barcodeHintLabel = nullptr;

    QLineEdit *barcodeInput = nullptr;

    QPushButton *startButton = nullptr;
    QPushButton *finishProductButton = nullptr;
    QPushButton *exitButton = nullptr;

    // ====================================================
    // Log
    // ====================================================

    QFrame *logCard = nullptr;
    QVBoxLayout *logCardLayout = nullptr;

    QLabel *logTitle = nullptr;
    QLabel *logSubTitle = nullptr;

    QTextEdit *logBox = nullptr;
    QPushButton *clearLogButton = nullptr;

    // ====================================================
    // 현재 상품 상태
    // ====================================================

    QString currentBarcode;

    double currentWeight = 0.0;

    bool hasWeight = false;

    int currentPhotoCount = 0;

    // ====================================================
    // Manager
    // ====================================================

    ScaleManager *scaleManager = nullptr;
    CameraManager *cameraManager = nullptr;

    // ====================================================
    // 작업 기능
    // ====================================================

    void handleBarcode();

    void refreshSerialPorts();

    void selectIncomingFolder();

    void selectWorkspaceFolder();

    void loadSettings();

    void saveSettings();

    void applyFolders();

    void resetCurrentProduct();

    // ====================================================
    // 상태 표시
    // ====================================================

    void setScaleStatus(
        const QString &text,
        bool connected
    );

    void setCameraStatus(
        const QString &text,
        bool active
    );

    // ====================================================
    // Log
    // ====================================================

    void writeLog(
        const QString &message,
        const QString &type = "SYSTEM"
    );

    // ====================================================
    // Responsive UI
    // ====================================================

    void updateResponsiveUi();

    void setWidgetFont(
        QWidget *widget,
        double basePointSize,
        bool bold,
        double scale
    );
};