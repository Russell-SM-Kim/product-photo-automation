#include "MainWindow.h"

#include "ScaleManager.h"
#include "CameraManager.h"
#include "FileManager.h"

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>

#include <QDateTime>
#include <QApplication>
#include <QFont>

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

#include <QSettings>

#include <QSerialPortInfo>

#include <QResizeEvent>

#include <QtGlobal>

#include <QIcon>


// ========================================================
// MainWindow
// ========================================================

MainWindow::MainWindow(
    QWidget *parent
)
    : QMainWindow(parent)
{
    // ====================================================
    // Window
    // ====================================================

    setWindowTitle(
        "상품 촬영 자동화"
    );

    setWindowIcon(
        QIcon(
            ":/assets/app_icon.png"
        )
    );

    resize(
        1180,
        900
    );

    // 너무 작아지면 작업 UI 자체가 의미가 없으므로
    // 최소 크기는 어느 정도 확보
    setMinimumSize(
        900,
        700
    );

    // ====================================================
    // Manager
    // ====================================================

    scaleManager =
        new ScaleManager(
            this
        );

    cameraManager =
        new CameraManager(
            this
        );

    // ====================================================
    // 기본 스타일
    // ====================================================

    setStyleSheet(
        R"(

        QMainWindow {
            background-color: #F4F7FB;
        }

        QWidget {
            font-family: "Malgun Gothic";
            color: #172033;
        }

        QLabel {
            background: transparent;
        }

        QFrame#Card {
            background-color: #FFFFFF;
            border: 1px solid #E1E7EF;
            border-radius: 12px;
        }

        QLineEdit {
            background-color: #FFFFFF;
            border: 1px solid #D7DFEA;
            border-radius: 7px;
            padding-left: 11px;
            padding-right: 11px;
        }

        QLineEdit:focus {
            border: 2px solid #2878F0;
        }

        QLineEdit:read-only {
            background-color: #F8FAFC;
            color: #475467;
        }

        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #D7DFEA;
            border-radius: 7px;
            padding-left: 11px;
            padding-right: 8px;
        }

        QComboBox:focus {
            border: 2px solid #2878F0;
        }

        QPushButton {
            background-color: #FFFFFF;

            border: 1px solid #D7DFEA;
            border-radius: 7px;

            color: #344054;

            padding-left: 12px;
            padding-right: 12px;
        }

        QPushButton:hover {
            background-color: #F4F7FB;
            border-color: #B8C3D3;
        }

        QPushButton:pressed {
            background-color: #E9EEF5;
        }

        QPushButton#PrimaryButton {
            background-color: #226FE8;
            border: none;
            color: #FFFFFF;
        }

        QPushButton#PrimaryButton:hover {
            background-color: #1763D7;
        }

        QPushButton#FinishButton {
            background-color: #FFF7ED;
            border: 1px solid #FED7AA;
            color: #C2410C;
        }

        QPushButton#FinishButton:hover {
            background-color: #FFEDD5;
        }

        QPushButton#ExitButton {
            background-color: #FFFFFF;
            border: 1px solid #FDA29B;
            color: #D92D20;
        }

        QPushButton#ExitButton:hover {
            background-color: #FFF1F1;
        }

        QTextEdit {
            background-color: #FBFCFE;

            border: 1px solid #E4EAF2;
            border-radius: 8px;

            padding: 9px;

            font-family: "Consolas", "Malgun Gothic";

            selection-background-color: #D7E6FF;
        }

        )"
    );

    // ====================================================
    // Central
    // ====================================================

    auto *centralWidget =
        new QWidget(
            this
        );

    setCentralWidget(
        centralWidget
    );

    auto *mainLayout =
        new QVBoxLayout(
            centralWidget
        );

    mainLayout->setContentsMargins(
        18,
        14,
        18,
        18
    );

    mainLayout->setSpacing(
        12
    );

    // ====================================================
    // HEADER
    // ====================================================

    auto *headerLayout =
        new QHBoxLayout();

    auto *titleLayout =
        new QVBoxLayout();

    titleLayout->setSpacing(
        1
    );

    titleLabel =
        new QLabel(
            "상품 촬영 자동화 시스템"
        );

    subTitleLabel =
        new QLabel(
            "Product Photo Automation System"
        );

    subTitleLabel->setStyleSheet(
        "color:#7A8699;"
    );

    titleLayout->addWidget(
        titleLabel
    );

    titleLayout->addWidget(
        subTitleLabel
    );

    headerLayout->addLayout(
        titleLayout
    );

    headerLayout->addStretch();

    systemStatusLabel =
        new QLabel(
            "●  작업 대기"
        );

    systemStatusLabel->setAlignment(
        Qt::AlignCenter
    );

    systemStatusLabel->setStyleSheet(
        "background:#EEF4FF;"
        "color:#175CD3;"
        "border:1px solid #D1E0FF;"
        "border-radius:16px;"
        "padding:6px 12px;"
    );

    headerLayout->addWidget(
        systemStatusLabel
    );

    mainLayout->addLayout(
        headerLayout
    );

    // ====================================================
    // SETTINGS CARD
    // ====================================================

    settingsCard =
        new QFrame();

    settingsCard->setObjectName(
        "Card"
    );

    settingsCardLayout =
        new QVBoxLayout(
            settingsCard
        );

    settingsCardLayout->setContentsMargins(
        18,
        14,
        18,
        14
    );

    settingsCardLayout->setSpacing(
        9
    );

    auto *settingsHeaderLayout =
        new QHBoxLayout();

    settingsTitle =
        new QLabel(
            "환경 설정"
        );

    settingsHint =
        new QLabel(
            "PC별 설정은 자동으로 저장됩니다."
        );

    settingsHint->setStyleSheet(
        "color:#98A2B3;"
    );

    settingsHeaderLayout->addWidget(
        settingsTitle
    );

    settingsHeaderLayout->addStretch();

    settingsHeaderLayout->addWidget(
        settingsHint
    );

    settingsCardLayout->addLayout(
        settingsHeaderLayout
    );

    auto *settingsGrid =
        new QGridLayout();

    settingsGrid->setHorizontalSpacing(
        10
    );

    settingsGrid->setVerticalSpacing(
        5
    );

    settingsGrid->setColumnStretch(
        1,
        1
    );

    // ----------------------------------------------------
    // SCALE PORT
    // ----------------------------------------------------

    scalePortTitle =
        new QLabel(
            "전자저울 포트"
        );

    serialPortCombo =
        new QComboBox();

    refreshPortsButton =
        new QPushButton(
            "새로고침"
        );

    settingsGrid->addWidget(
        scalePortTitle,
        0,
        0
    );

    settingsGrid->addWidget(
        serialPortCombo,
        0,
        1
    );

    settingsGrid->addWidget(
        refreshPortsButton,
        0,
        2
    );

    // ----------------------------------------------------
    // INCOMING
    // ----------------------------------------------------

    incomingTitle =
        new QLabel(
            "카메라 수신 폴더"
        );

    incomingFolderEdit =
        new QLineEdit();

    incomingFolderEdit->setReadOnly(
        true
    );

    incomingFolderEdit->setPlaceholderText(
        "Sony Imaging Edge 수신 폴더"
    );

    browseIncomingButton =
        new QPushButton(
            "폴더 선택"
        );

    settingsGrid->addWidget(
        incomingTitle,
        1,
        0
    );

    settingsGrid->addWidget(
        incomingFolderEdit,
        1,
        1
    );

    settingsGrid->addWidget(
        browseIncomingButton,
        1,
        2
    );

    // ----------------------------------------------------
    // WORKSPACE
    // ----------------------------------------------------

    workspaceTitle =
        new QLabel(
            "작업 저장 폴더"
        );

    auto *workspaceLayout =
        new QVBoxLayout();

    workspaceLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    workspaceLayout->setSpacing(
        2
    );

    workspaceFolderEdit =
        new QLineEdit();

    workspaceFolderEdit->setReadOnly(
        true
    );

    workspaceFolderEdit->setPlaceholderText(
        "Products / Data가 생성될 상위 폴더"
    );

    workspaceHint =
        new QLabel(
            "선택한 경로 아래에 Products 및 Data 폴더가 자동 생성됩니다."
        );

    workspaceHint->setStyleSheet(
        "color:#98A2B3;"
        "padding-left:3px;"
    );

    workspaceLayout->addWidget(
        workspaceFolderEdit
    );

    workspaceLayout->addWidget(
        workspaceHint
    );

    browseWorkspaceButton =
        new QPushButton(
            "폴더 선택"
        );

    settingsGrid->addWidget(
        workspaceTitle,
        2,
        0
    );

    settingsGrid->addLayout(
        workspaceLayout,
        2,
        1
    );

    settingsGrid->addWidget(
        browseWorkspaceButton,
        2,
        2,
        Qt::AlignTop
    );

    settingsCardLayout->addLayout(
        settingsGrid
    );

    mainLayout->addWidget(
        settingsCard
    );

    // ====================================================
    // DASHBOARD
    // ====================================================

    auto *dashboardLayout =
        new QHBoxLayout();

    dashboardLayout->setSpacing(
        12
    );

    // ====================================================
    // DEVICE CARD
    // ====================================================

    deviceCard =
        new QFrame();

    deviceCard->setObjectName(
        "Card"
    );

    deviceCardLayout =
        new QVBoxLayout(
            deviceCard
        );

    deviceCardLayout->setContentsMargins(
        18,
        14,
        18,
        14
    );

    deviceCardLayout->setSpacing(
        10
    );

    deviceTitle =
        new QLabel(
            "장치 상태"
        );

    deviceCardLayout->addWidget(
        deviceTitle
    );

    // ----------------------------------------------------
    // SCALE DEVICE
    // ----------------------------------------------------

    auto *scaleRow =
        new QHBoxLayout();

    scaleDotLabel =
        new QLabel(
            "●"
        );

    scaleDotLabel->setStyleSheet(
        "color:#B8C1CC;"
    );

    auto *scaleInfoLayout =
        new QVBoxLayout();

    scaleInfoLayout->setSpacing(
        0
    );

    scaleDeviceNameLabel =
        new QLabel(
            "전자저울"
        );

    scaleStatusLabel =
        new QLabel(
            "연결 안 됨"
        );

    scaleStatusLabel->setStyleSheet(
        "color:#98A2B3;"
    );

    scaleInfoLayout->addWidget(
        scaleDeviceNameLabel
    );

    scaleInfoLayout->addWidget(
        scaleStatusLabel
    );

    scaleRow->addWidget(
        scaleDotLabel
    );

    scaleRow->addLayout(
        scaleInfoLayout
    );

    scaleRow->addStretch();

    deviceCardLayout->addLayout(
        scaleRow
    );

    // ----------------------------------------------------
    // SEPARATOR
    // ----------------------------------------------------

    auto *separator =
        new QFrame();

    separator->setFrameShape(
        QFrame::HLine
    );

    separator->setStyleSheet(
        "color:#EEF1F5;"
    );

    deviceCardLayout->addWidget(
        separator
    );

    // ----------------------------------------------------
    // CAMERA DEVICE
    // ----------------------------------------------------

    auto *cameraRow =
        new QHBoxLayout();

    cameraDotLabel =
        new QLabel(
            "●"
        );

    cameraDotLabel->setStyleSheet(
        "color:#B8C1CC;"
    );

    auto *cameraInfoLayout =
        new QVBoxLayout();

    cameraInfoLayout->setSpacing(
        0
    );

    cameraDeviceNameLabel =
        new QLabel(
            "카메라"
        );

    cameraStatusLabel =
        new QLabel(
            "폴더 감시 대기"
        );

    cameraStatusLabel->setStyleSheet(
        "color:#98A2B3;"
    );

    cameraInfoLayout->addWidget(
        cameraDeviceNameLabel
    );

    cameraInfoLayout->addWidget(
        cameraStatusLabel
    );

    cameraRow->addWidget(
        cameraDotLabel
    );

    cameraRow->addLayout(
        cameraInfoLayout
    );

    cameraRow->addStretch();

    deviceCardLayout->addLayout(
        cameraRow
    );

    deviceCardLayout->addStretch();

    dashboardLayout->addWidget(
        deviceCard,
        1
    );

    // ====================================================
    // PRODUCT CARD
    // ====================================================

    productCard =
        new QFrame();

    productCard->setObjectName(
        "Card"
    );

    productCardLayout =
        new QVBoxLayout(
            productCard
        );

    productCardLayout->setContentsMargins(
        18,
        14,
        18,
        14
    );

    productCardLayout->setSpacing(
        7
    );

    auto *productHeader =
        new QHBoxLayout();

    productTitle =
        new QLabel(
            "현재 상품"
        );

    activeBadgeLabel =
        new QLabel(
            "ACTIVE PRODUCT"
        );

    activeBadgeLabel->setAlignment(
        Qt::AlignCenter
    );

    activeBadgeLabel->setStyleSheet(
        "background:#EEF4FF;"
        "color:#175CD3;"
        "border-radius:10px;"
        "padding:3px 8px;"
    );

    productHeader->addWidget(
        productTitle
    );

    productHeader->addStretch();

    productHeader->addWidget(
        activeBadgeLabel
    );

    productCardLayout->addLayout(
        productHeader
    );

    barcodeCaptionLabel =
        new QLabel(
            "BARCODE"
        );

    barcodeCaptionLabel->setStyleSheet(
        "color:#98A2B3;"
    );

    productCardLayout->addWidget(
        barcodeCaptionLabel
    );

    barcodeValueLabel =
        new QLabel(
            "-"
        );

    barcodeValueLabel->setMinimumHeight(
        32
    );

    productCardLayout->addWidget(
        barcodeValueLabel
    );

    auto *productStatsLayout =
        new QHBoxLayout();

    productStatsLayout->setSpacing(
        12
    );

    // ----------------------------------------------------
    // WEIGHT BOX
    // ----------------------------------------------------

    weightBox =
        new QFrame();

    weightBox->setStyleSheet(
        "QFrame {"
        "background:#F8FAFC;"
        "border:1px solid #EEF1F5;"
        "border-radius:8px;"
        "}"
    );

    auto *weightLayout =
        new QVBoxLayout(
            weightBox
        );

    weightLayout->setContentsMargins(
        14,
        8,
        14,
        8
    );

    weightLayout->setSpacing(
        2
    );

    weightCaptionLabel =
        new QLabel(
            "현재 중량"
        );

    weightCaptionLabel->setStyleSheet(
        "color:#7A8699;"
        "border:none;"
    );

    weightValueLabel =
        new QLabel(
            "- kg"
        );

    weightValueLabel->setStyleSheet(
        "color:#101828;"
        "border:none;"
    );

    weightValueLabel->setMinimumHeight(
        30
    );

    weightLayout->addWidget(
        weightCaptionLabel
    );

    weightLayout->addWidget(
        weightValueLabel
    );

    // ----------------------------------------------------
    // PHOTO BOX
    // ----------------------------------------------------

    photoBox =
        new QFrame();

    photoBox->setStyleSheet(
        "QFrame {"
        "background:#F8FAFC;"
        "border:1px solid #EEF1F5;"
        "border-radius:8px;"
        "}"
    );

    auto *photoLayout =
        new QVBoxLayout(
            photoBox
        );

    photoLayout->setContentsMargins(
        14,
        8,
        14,
        8
    );

    photoLayout->setSpacing(
        2
    );

    photoCaptionLabel =
        new QLabel(
            "촬영 이미지"
        );

    photoCaptionLabel->setStyleSheet(
        "color:#7A8699;"
        "border:none;"
    );

    photoCountLabel =
        new QLabel(
            "0장"
        );

    photoCountLabel->setStyleSheet(
        "color:#226FE8;"
        "border:none;"
    );

    photoCountLabel->setMinimumHeight(
        30
    );

    photoLayout->addWidget(
        photoCaptionLabel
    );

    photoLayout->addWidget(
        photoCountLabel
    );

    productStatsLayout->addWidget(
        weightBox,
        1
    );

    productStatsLayout->addWidget(
        photoBox,
        1
    );

    productCardLayout->addLayout(
        productStatsLayout
    );

    dashboardLayout->addWidget(
        productCard,
        2
    );

    mainLayout->addLayout(
        dashboardLayout
    );

    // ====================================================
    // BARCODE CARD
    // ====================================================

    barcodeCard =
        new QFrame();

    barcodeCard->setObjectName(
        "Card"
    );

    barcodeCardLayout =
        new QVBoxLayout(
            barcodeCard
        );

    barcodeCardLayout->setContentsMargins(
        18,
        13,
        18,
        13
    );

    barcodeCardLayout->setSpacing(
        7
    );

    auto *barcodeHeader =
        new QHBoxLayout();

    barcodeInputTitle =
        new QLabel(
            "바코드 스캔"
        );

    barcodeHintLabel =
        new QLabel(
            "USB 바코드 리더기로 스캔하면 자동 처리됩니다."
        );

    barcodeHintLabel->setStyleSheet(
        "color:#98A2B3;"
    );

    barcodeHeader->addWidget(
        barcodeInputTitle
    );

    barcodeHeader->addStretch();

    barcodeHeader->addWidget(
        barcodeHintLabel
    );

    barcodeCardLayout->addLayout(
        barcodeHeader
    );

    barcodeInput =
        new QLineEdit();

    barcodeInput->setPlaceholderText(
        "바코드를 스캔하세요..."
    );

    barcodeInput->setStyleSheet(
        "QLineEdit {"
        "background:#FFFFFF;"
        "border:1px solid #CBD5E1;"
        "border-radius:8px;"
        "padding-left:14px;"
        "}"
        "QLineEdit:focus {"
        "border:2px solid #226FE8;"
        "}"
    );

    barcodeCardLayout->addWidget(
        barcodeInput
    );

    auto *buttonLayout =
        new QHBoxLayout();

    buttonLayout->setSpacing(
        10
    );

    startButton =
        new QPushButton(
            "작업 시작"
        );

    startButton->setObjectName(
        "PrimaryButton"
    );

    finishProductButton =
        new QPushButton(
            "상품 종료"
        );

    finishProductButton->setObjectName(
        "FinishButton"
    );

    exitButton =
        new QPushButton(
            "프로그램 종료"
        );

    exitButton->setObjectName(
        "ExitButton"
    );

    buttonLayout->addWidget(
        startButton,
        2
    );

    buttonLayout->addWidget(
        finishProductButton,
        1
    );

    buttonLayout->addWidget(
        exitButton,
        1
    );

    barcodeCardLayout->addLayout(
        buttonLayout
    );

    mainLayout->addWidget(
        barcodeCard
    );

    // ====================================================
    // LOG CARD
    // ====================================================

    logCard =
        new QFrame();

    logCard->setObjectName(
        "Card"
    );

    logCardLayout =
        new QVBoxLayout(
            logCard
        );

    logCardLayout->setContentsMargins(
        18,
        13,
        18,
        14
    );

    logCardLayout->setSpacing(
        7
    );

    auto *logHeader =
        new QHBoxLayout();

    logTitle =
        new QLabel(
            "작업 로그"
        );

    logSubTitle =
        new QLabel(
            "장치 연결 · 바코드 · 촬영 · 저장 상태"
        );

    logSubTitle->setStyleSheet(
        "color:#98A2B3;"
    );

    clearLogButton =
        new QPushButton(
            "로그 지우기"
        );

    logHeader->addWidget(
        logTitle
    );

    logHeader->addWidget(
        logSubTitle
    );

    logHeader->addStretch();

    logHeader->addWidget(
        clearLogButton
    );

    logCardLayout->addLayout(
        logHeader
    );

    logBox =
        new QTextEdit();

    logBox->setReadOnly(
        true
    );

    logBox->setMinimumHeight(
        120
    );

    logCardLayout->addWidget(
        logBox
    );

    mainLayout->addWidget(
        logCard,
        1
    );

    // ====================================================
    // SIGNAL : PORT REFRESH
    // ====================================================

    connect(
        refreshPortsButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            refreshSerialPorts();

            writeLog(
                "사용 가능한 COM 포트를 새로고침했습니다.",
                "SYSTEM"
            );
        }
    );

    // ====================================================
    // SIGNAL : INCOMING
    // ====================================================

    connect(
        browseIncomingButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            selectIncomingFolder();
        }
    );

    // ====================================================
    // SIGNAL : WORKSPACE
    // ====================================================

    connect(
        browseWorkspaceButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            selectWorkspaceFolder();
        }
    );

    // ====================================================
    // SIGNAL : BARCODE
    // ====================================================

    connect(
        barcodeInput,
        &QLineEdit::returnPressed,
        this,
        &MainWindow::handleBarcode
    );

    // ====================================================
    // SIGNAL : START
    // ====================================================

    connect(
        startButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            saveSettings();

            applyFolders();

            // --------------------------------------------
            // PORT
            // --------------------------------------------

            if (
                serialPortCombo->currentIndex() < 0
            )
            {
                writeLog(
                    "전자저울 포트를 선택해주세요.",
                    "ERROR"
                );

                return;
            }

            QString portName =
                serialPortCombo
                    ->currentData()
                    .toString();

            if (
                portName.isEmpty()
            )
            {
                writeLog(
                    "전자저울 포트가 올바르지 않습니다.",
                    "ERROR"
                );

                return;
            }

            // --------------------------------------------
            // INCOMING
            // --------------------------------------------

            if (
                incomingFolderEdit
                    ->text()
                    .trimmed()
                    .isEmpty()
            )
            {
                writeLog(
                    "카메라 수신 폴더를 선택해주세요.",
                    "ERROR"
                );

                return;
            }

            // --------------------------------------------
            // WORKSPACE
            // --------------------------------------------

            if (
                workspaceFolderEdit
                    ->text()
                    .trimmed()
                    .isEmpty()
            )
            {
                writeLog(
                    "작업 저장 폴더를 선택해주세요.",
                    "ERROR"
                );

                return;
            }

            systemStatusLabel->setText(
                "●  작업 중"
            );

            systemStatusLabel->setStyleSheet(
                "background:#ECFDF3;"
                "color:#027A48;"
                "border:1px solid #ABEFC6;"
                "border-radius:16px;"
                "padding:6px 12px;"
            );

            writeLog(
                "상품 촬영 작업을 시작합니다.",
                "SYSTEM"
            );

            if (
                !scaleManager->isConnected()
            )
            {
                writeLog(
                    QString(
                        "%1 연결을 시도합니다."
                    )
                        .arg(
                            portName
                        ),
                    "DEVICE"
                );

                scaleManager
                    ->connectScale(
                        portName
                    );
            }

            if (
                !cameraManager->isWatching()
            )
            {
                cameraManager
                    ->startWatching();
            }

            barcodeInput->setFocus();
        }
    );

    // ====================================================
    // SIGNAL : PRODUCT FINISH
    // ====================================================

    connect(
        finishProductButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (
                currentBarcode.isEmpty()
            )
            {
                writeLog(
                    "종료할 상품이 없습니다.",
                    "SYSTEM"
                );

                return;
            }

            QString finishedBarcode =
                currentBarcode;

            writeLog(
                QString(
                    "%1 상품 종료 · 촬영 %2장"
                )
                    .arg(
                        finishedBarcode
                    )
                    .arg(
                        currentPhotoCount
                    ),
                "BARCODE"
            );

            resetCurrentProduct();

            barcodeInput->clear();

            barcodeInput->setFocus();
        }
    );

    // ====================================================
    // SIGNAL : EXIT
    // ====================================================

    connect(
        exitButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            saveSettings();

            cameraManager
                ->stopWatching();

            scaleManager
                ->disconnectScale();

            QApplication::quit();
        }
    );

    // ====================================================
    // SIGNAL : CLEAR LOG
    // ====================================================

    connect(
        clearLogButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            logBox->clear();

            writeLog(
                "작업 로그를 초기화했습니다.",
                "SYSTEM"
            );

            barcodeInput->setFocus();
        }
    );

    // ====================================================
    // SCALE : CONNECTED
    // ====================================================

    connect(
        scaleManager,
        &ScaleManager::connected,
        this,
        [this](
            const QString &portName
        )
        {
            setScaleStatus(
                QString(
                    "%1 연결됨"
                )
                    .arg(
                        portName
                    ),
                true
            );

            writeLog(
                QString(
                    "전자저울 %1 연결 완료"
                )
                    .arg(
                        portName
                    ),
                "DEVICE"
            );
        }
    );

    // ====================================================
    // SCALE : DISCONNECTED
    // ====================================================

    connect(
        scaleManager,
        &ScaleManager::disconnected,
        this,
        [this]()
        {
            setScaleStatus(
                "연결 안 됨",
                false
            );

            weightValueLabel->setText(
                "- kg"
            );

            hasWeight =
                false;

            writeLog(
                "전자저울 연결이 종료되었습니다.",
                "DEVICE"
            );
        }
    );

    // ====================================================
    // SCALE : WEIGHT
    // ====================================================

    connect(
        scaleManager,
        &ScaleManager::weightChanged,
        this,
        [this](
            double weight
        )
        {
            currentWeight =
                weight;

            hasWeight =
                true;

            weightValueLabel->setText(
                QString(
                    "%1 kg"
                )
                    .arg(
                        currentWeight,
                        0,
                        'f',
                        2
                    )
            );
        }
    );

    // ====================================================
    // SCALE : ERROR
    // ====================================================

    connect(
        scaleManager,
        &ScaleManager::errorOccurred,
        this,
        [this](
            const QString &message
        )
        {
            setScaleStatus(
                "연결 오류",
                false
            );

            writeLog(
                message,
                "ERROR"
            );
        }
    );

    // ====================================================
    // CAMERA : START
    // ====================================================

    connect(
        cameraManager,
        &CameraManager::watchingStarted,
        this,
        [this](
            const QString &folderPath
        )
        {
            setCameraStatus(
                "수신 폴더 감시 중",
                true
            );

            writeLog(
                QString(
                    "촬영 폴더 감시 시작 · %1"
                )
                    .arg(
                        QDir::toNativeSeparators(
                            folderPath
                        )
                    ),
                "CAMERA"
            );
        }
    );

    // ====================================================
    // CAMERA : STOP
    // ====================================================

    connect(
        cameraManager,
        &CameraManager::watchingStopped,
        this,
        [this]()
        {
            setCameraStatus(
                "폴더 감시 중지",
                false
            );

            writeLog(
                "카메라 촬영 폴더 감시가 종료되었습니다.",
                "CAMERA"
            );
        }
    );

    // ====================================================
    // CAMERA : PHOTO DETECTED
    // ====================================================

    connect(
        cameraManager,
        &CameraManager::photoDetected,
        this,
        [this](
            const QString &filePath
        )
        {
            QString fileName =
                QFileInfo(
                    filePath
                ).fileName();

            writeLog(
                QString(
                    "%1 감지"
                )
                    .arg(
                        fileName
                    ),
                "PHOTO"
            );
        }
    );

    // ====================================================
    // CAMERA : PHOTO SAVED
    // ====================================================

    connect(
        cameraManager,
        &CameraManager::photoSaved,
        this,
        [this](
            const QString &barcode,
            const QString &savedPath,
            int photoIndex
        )
        {
            if (
                barcode == currentBarcode
            )
            {
                currentPhotoCount =
                    photoIndex;

                photoCountLabel->setText(
                    QString(
                        "%1장"
                    )
                        .arg(
                            currentPhotoCount
                        )
                );
            }

            QString savedFileName =
                QFileInfo(
                    savedPath
                ).fileName();

            writeLog(
                QString(
                    "%1 저장 완료"
                )
                    .arg(
                        savedFileName
                    ),
                "SAVE"
            );
        }
    );

    // ====================================================
    // CAMERA : ERROR
    // ====================================================

    connect(
        cameraManager,
        &CameraManager::errorOccurred,
        this,
        [this](
            const QString &message
        )
        {
            setCameraStatus(
                "오류 발생",
                false
            );

            writeLog(
                QString(
                    "카메라 오류 · %1"
                )
                    .arg(
                        message
                    ),
                "ERROR"
            );
        }
    );

    // ====================================================
    // INITIALIZE
    // ====================================================

    refreshSerialPorts();

    loadSettings();

    applyFolders();

    writeLog(
        "프로그램 실행",
        "SYSTEM"
    );

    writeLog(
        "환경 설정을 확인한 뒤 작업 시작 버튼을 눌러주세요.",
        "SYSTEM"
    );

    // 첫 UI 배율 적용
    updateResponsiveUi();

    barcodeInput->setFocus();
}


// ========================================================
// Resize Event
// ========================================================

void MainWindow::resizeEvent(
    QResizeEvent *event
)
{
    QMainWindow::resizeEvent(
        event
    );

    updateResponsiveUi();
}


// ========================================================
// Responsive UI
// ========================================================

void MainWindow::updateResponsiveUi()
{
    // 기준 UI 크기
    constexpr double baseWidth =
        1180.0;

    constexpr double baseHeight =
        900.0;

    double widthScale =
        static_cast<double>(
            width()
        ) /
        baseWidth;

    double heightScale =
        static_cast<double>(
            height()
        ) /
        baseHeight;

    // 가로/세로 중 작은 쪽을 기준으로 잡음
    double scale =
        qMin(
            widthScale,
            heightScale
        );

    // 너무 작거나 너무 커지지 않도록 제한
    scale =
        qBound(
            0.72,
            scale,
            1.18
        );

    // ====================================================
    // Header
    // ====================================================

    setWidgetFont(
        titleLabel,
        21.0,
        true,
        scale
    );

    setWidgetFont(
        subTitleLabel,
        10.0,
        false,
        scale
    );

    setWidgetFont(
        systemStatusLabel,
        10.0,
        true,
        scale
    );

    // ====================================================
    // Section titles
    // ====================================================

    setWidgetFont(
        settingsTitle,
        12.0,
        true,
        scale
    );

    setWidgetFont(
        deviceTitle,
        12.0,
        true,
        scale
    );

    setWidgetFont(
        productTitle,
        12.0,
        true,
        scale
    );

    setWidgetFont(
        barcodeInputTitle,
        12.0,
        true,
        scale
    );

    setWidgetFont(
        logTitle,
        12.0,
        true,
        scale
    );

    // ====================================================
    // Settings
    // ====================================================

    setWidgetFont(
        settingsHint,
        9.0,
        false,
        scale
    );

    setWidgetFont(
        scalePortTitle,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        incomingTitle,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        workspaceTitle,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        workspaceHint,
        8.0,
        false,
        scale
    );

    setWidgetFont(
        serialPortCombo,
        10.0,
        false,
        scale
    );

    setWidgetFont(
        incomingFolderEdit,
        10.0,
        false,
        scale
    );

    setWidgetFont(
        workspaceFolderEdit,
        10.0,
        false,
        scale
    );

    setWidgetFont(
        refreshPortsButton,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        browseIncomingButton,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        browseWorkspaceButton,
        10.0,
        true,
        scale
    );

    // ====================================================
    // Device
    // ====================================================

    setWidgetFont(
        scaleDotLabel,
        12.0,
        false,
        scale
    );

    setWidgetFont(
        cameraDotLabel,
        12.0,
        false,
        scale
    );

    setWidgetFont(
        scaleDeviceNameLabel,
        11.0,
        true,
        scale
    );

    setWidgetFont(
        cameraDeviceNameLabel,
        11.0,
        true,
        scale
    );

    setWidgetFont(
        scaleStatusLabel,
        9.0,
        false,
        scale
    );

    setWidgetFont(
        cameraStatusLabel,
        9.0,
        false,
        scale
    );

    // ====================================================
    // Product
    // ====================================================

    setWidgetFont(
        activeBadgeLabel,
        8.0,
        true,
        scale
    );

    setWidgetFont(
        barcodeCaptionLabel,
        8.0,
        true,
        scale
    );

    setWidgetFont(
        barcodeValueLabel,
        20.0,
        true,
        scale
    );

    setWidgetFont(
        weightCaptionLabel,
        8.0,
        false,
        scale
    );

    setWidgetFont(
        photoCaptionLabel,
        8.0,
        false,
        scale
    );

    setWidgetFont(
        weightValueLabel,
        17.0,
        true,
        scale
    );

    setWidgetFont(
        photoCountLabel,
        17.0,
        true,
        scale
    );

    // ====================================================
    // Barcode
    // ====================================================

    setWidgetFont(
        barcodeHintLabel,
        8.0,
        false,
        scale
    );

    setWidgetFont(
        barcodeInput,
        13.0,
        true,
        scale
    );

    setWidgetFont(
        startButton,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        finishProductButton,
        10.0,
        true,
        scale
    );

    setWidgetFont(
        exitButton,
        10.0,
        true,
        scale
    );

    // ====================================================
    // Log
    // ====================================================

    setWidgetFont(
        logSubTitle,
        8.0,
        false,
        scale
    );

    setWidgetFont(
        clearLogButton,
        9.0,
        true,
        scale
    );

    setWidgetFont(
        logBox,
        9.0,
        false,
        scale
    );

    // ====================================================
    // Widget Height
    // ====================================================

    int inputHeight =
        qRound(
            38.0 * scale
        );

    inputHeight =
        qBound(
            30,
            inputHeight,
            44
        );

    serialPortCombo->setMinimumHeight(
        inputHeight
    );

    serialPortCombo->setMaximumHeight(
        inputHeight
    );

    incomingFolderEdit->setMinimumHeight(
        inputHeight
    );

    incomingFolderEdit->setMaximumHeight(
        inputHeight
    );

    workspaceFolderEdit->setMinimumHeight(
        inputHeight
    );

    workspaceFolderEdit->setMaximumHeight(
        inputHeight
    );

    refreshPortsButton->setMinimumHeight(
        inputHeight
    );

    refreshPortsButton->setMaximumHeight(
        inputHeight
    );

    browseIncomingButton->setMinimumHeight(
        inputHeight
    );

    browseIncomingButton->setMaximumHeight(
        inputHeight
    );

    browseWorkspaceButton->setMinimumHeight(
        inputHeight
    );

    browseWorkspaceButton->setMaximumHeight(
        inputHeight
    );

    // ====================================================
    // Barcode Input Height
    // ====================================================

    int barcodeHeight =
        qRound(
            44.0 * scale
        );

    barcodeHeight =
        qBound(
            34,
            barcodeHeight,
            50
        );

    barcodeInput->setMinimumHeight(
        barcodeHeight
    );

    barcodeInput->setMaximumHeight(
        barcodeHeight
    );

    // ====================================================
    // Main Buttons
    // ====================================================

    int buttonHeight =
        qRound(
            42.0 * scale
        );

    buttonHeight =
        qBound(
            32,
            buttonHeight,
            48
        );

    startButton->setMinimumHeight(
        buttonHeight
    );

    startButton->setMaximumHeight(
        buttonHeight
    );

    finishProductButton->setMinimumHeight(
        buttonHeight
    );

    finishProductButton->setMaximumHeight(
        buttonHeight
    );

    exitButton->setMinimumHeight(
        buttonHeight
    );

    exitButton->setMaximumHeight(
        buttonHeight
    );

    // ====================================================
    // Clear Log Button
    // ====================================================

    int smallButtonHeight =
        qRound(
            34.0 * scale
        );

    smallButtonHeight =
        qBound(
            28,
            smallButtonHeight,
            38
        );

    clearLogButton->setMinimumHeight(
        smallButtonHeight
    );

    clearLogButton->setMaximumHeight(
        smallButtonHeight
    );

    // ====================================================
    // Product value heights
    // ====================================================

    int valueHeight =
        qRound(
            32.0 * scale
        );

    valueHeight =
        qBound(
            24,
            valueHeight,
            38
        );

    barcodeValueLabel->setMinimumHeight(
        valueHeight
    );

    weightValueLabel->setMinimumHeight(
        valueHeight
    );

    photoCountLabel->setMinimumHeight(
        valueHeight
    );

    // ====================================================
    // Card Padding
    // ====================================================

    int horizontalPadding =
        qRound(
            18.0 * scale
        );

    int verticalPadding =
        qRound(
            14.0 * scale
        );

    horizontalPadding =
        qBound(
            10,
            horizontalPadding,
            20
        );

    verticalPadding =
        qBound(
            8,
            verticalPadding,
            16
        );

    settingsCardLayout->setContentsMargins(
        horizontalPadding,
        verticalPadding,
        horizontalPadding,
        verticalPadding
    );

    deviceCardLayout->setContentsMargins(
        horizontalPadding,
        verticalPadding,
        horizontalPadding,
        verticalPadding
    );

    productCardLayout->setContentsMargins(
        horizontalPadding,
        verticalPadding,
        horizontalPadding,
        verticalPadding
    );

    barcodeCardLayout->setContentsMargins(
        horizontalPadding,
        verticalPadding,
        horizontalPadding,
        verticalPadding
    );

    logCardLayout->setContentsMargins(
        horizontalPadding,
        verticalPadding,
        horizontalPadding,
        verticalPadding
    );

    // ====================================================
    // Log minimum height
    // ====================================================

    int logMinimumHeight =
        qRound(
            120.0 * scale
        );

    logMinimumHeight =
        qBound(
            90,
            logMinimumHeight,
            150
        );

    logBox->setMinimumHeight(
        logMinimumHeight
    );
}


// ========================================================
// Font Helper
// ========================================================

void MainWindow::setWidgetFont(
    QWidget *widget,
    double basePointSize,
    bool bold,
    double scale
)
{
    if (
        widget == nullptr
    )
    {
        return;
    }

    QFont font =
        widget->font();

    double pointSize =
        basePointSize * scale;

    pointSize =
        qBound(
            7.0,
            pointSize,
            basePointSize * 1.18
        );

    font.setPointSizeF(
        pointSize
    );

    font.setBold(
        bold
    );

    widget->setFont(
        font
    );
}


// ========================================================
// Barcode
// ========================================================

void MainWindow::handleBarcode()
{
    QString barcode =
        barcodeInput
            ->text()
            .trimmed();

    if (
        barcode.isEmpty()
    )
    {
        return;
    }

    // ====================================================
    // 이전 상품 자동 종료
    // ====================================================

    if (
        !currentBarcode.isEmpty() &&
        currentBarcode != barcode
    )
    {
        writeLog(
            QString(
                "%1 자동 종료 · 촬영 %2장"
            )
                .arg(
                    currentBarcode
                )
                .arg(
                    currentPhotoCount
                ),
            "BARCODE"
        );
    }

    bool isNewProduct =
        currentBarcode != barcode;

    currentBarcode =
        barcode;

    if (
        isNewProduct
    )
    {
        currentPhotoCount =
            0;

        photoCountLabel->setText(
            "0장"
        );
    }

    barcodeValueLabel->setText(
        currentBarcode
    );

    cameraManager->setCurrentBarcode(
        currentBarcode
    );

    // ====================================================
    // Weight
    // ====================================================

    if (
        hasWeight
    )
    {
        writeLog(
            QString(
                "%1 인식 · %2 kg"
            )
                .arg(
                    currentBarcode
                )
                .arg(
                    currentWeight,
                    0,
                    'f',
                    2
                ),
            "BARCODE"
        );

        QString errorMessage;

        bool saved =
            FileManager::saveProductData(
                currentBarcode,
                currentWeight,
                &errorMessage
            );

        if (
            saved
        )
        {
            writeLog(
                QString(
                    "%1 상품 데이터 저장 완료"
                )
                    .arg(
                        currentBarcode
                    ),
                "SAVE"
            );
        }
        else
        {
            writeLog(
                QString(
                    "상품 데이터 저장 실패 · %1"
                )
                    .arg(
                        errorMessage
                    ),
                "ERROR"
            );
        }
    }
    else
    {
        writeLog(
            QString(
                "%1 인식 · 중량 데이터 없음"
            )
                .arg(
                    currentBarcode
                ),
            "BARCODE"
        );
    }

    barcodeInput->clear();

    barcodeInput->setFocus();
}


// ========================================================
// Serial Port
// ========================================================

void MainWindow::refreshSerialPorts()
{
    QString previousPort =
        serialPortCombo
            ->currentData()
            .toString();

    serialPortCombo->clear();

    const QList<QSerialPortInfo> ports =
        QSerialPortInfo::availablePorts();

    for (
        const QSerialPortInfo &port :
        ports
    )
    {
        QString displayName =
            port.portName();

        if (
            !port.description().isEmpty()
        )
        {
            displayName +=
                " - " +
                port.description();
        }

        serialPortCombo->addItem(
            displayName,
            port.portName()
        );
    }

    if (
        serialPortCombo->count() == 0
    )
    {
        serialPortCombo->addItem(
            "사용 가능한 COM 포트 없음",
            ""
        );

        return;
    }

    if (
        !previousPort.isEmpty()
    )
    {
        int index =
            serialPortCombo->findData(
                previousPort
            );

        if (
            index >= 0
        )
        {
            serialPortCombo->setCurrentIndex(
                index
            );
        }
    }
}


// ========================================================
// Incoming Folder
// ========================================================

void MainWindow::selectIncomingFolder()
{
    QString initialFolder =
        incomingFolderEdit->text();

    QString folder =
        QFileDialog::getExistingDirectory(
            this,
            "카메라 수신 폴더 선택",
            initialFolder
        );

    if (
        folder.isEmpty()
    )
    {
        return;
    }

    QString nativeFolder =
        QDir::toNativeSeparators(
            folder
        );

    incomingFolderEdit->setText(
        nativeFolder
    );

    incomingFolderEdit->setToolTip(
        nativeFolder
    );

    applyFolders();

    saveSettings();

    writeLog(
        QString(
            "카메라 수신 폴더 변경 · %1"
        )
            .arg(
                nativeFolder
            ),
        "SYSTEM"
    );
}


// ========================================================
// Workspace Folder
// ========================================================

void MainWindow::selectWorkspaceFolder()
{
    QString initialFolder =
        workspaceFolderEdit->text();

    QString folder =
        QFileDialog::getExistingDirectory(
            this,
            "작업 저장 폴더 선택",
            initialFolder
        );

    if (
        folder.isEmpty()
    )
    {
        return;
    }

    QString nativeFolder =
        QDir::toNativeSeparators(
            folder
        );

    workspaceFolderEdit->setText(
        nativeFolder
    );

    workspaceFolderEdit->setToolTip(
        nativeFolder
    );

    applyFolders();

    saveSettings();

    writeLog(
        QString(
            "작업 저장 폴더 변경 · %1"
        )
            .arg(
                nativeFolder
            ),
        "SYSTEM"
    );
}


// ========================================================
// Apply Folder
// ========================================================

void MainWindow::applyFolders()
{
    QString incomingFolder =
        incomingFolderEdit
            ->text()
            .trimmed();

    QString workspaceFolder =
        workspaceFolderEdit
            ->text()
            .trimmed();

    if (
        !incomingFolder.isEmpty()
    )
    {
        cameraManager->setIncomingFolder(
            incomingFolder
        );

        incomingFolderEdit->setToolTip(
            incomingFolder
        );
    }

    if (
        !workspaceFolder.isEmpty()
    )
    {
        QString productsFolder =
            QDir(
                workspaceFolder
            ).filePath(
                "Products"
            );

        QString dataFolder =
            QDir(
                workspaceFolder
            ).filePath(
                "Data"
            );

        QDir().mkpath(
            productsFolder
        );

        QDir().mkpath(
            dataFolder
        );

        cameraManager->setProductsFolder(
            productsFolder
        );

        FileManager::setDataFolder(
            dataFolder
        );

        workspaceFolderEdit->setToolTip(
            workspaceFolder
        );
    }
}


// ========================================================
// Load Settings
// ========================================================

void MainWindow::loadSettings()
{
    QSettings settings(
        "ProductPhotoAutomation",
        "ProductPhotoAutomation"
    );

    QString savedPort =
        settings
            .value(
                "scalePort"
            )
            .toString();

    QString savedIncoming =
        settings
            .value(
                "incomingFolder"
            )
            .toString();

    QString savedWorkspace =
        settings
            .value(
                "workspaceFolder"
            )
            .toString();

    if (
        !savedPort.isEmpty()
    )
    {
        int index =
            serialPortCombo->findData(
                savedPort
            );

        if (
            index >= 0
        )
        {
            serialPortCombo->setCurrentIndex(
                index
            );
        }
    }

    if (
        !savedIncoming.isEmpty()
    )
    {
        incomingFolderEdit->setText(
            savedIncoming
        );

        incomingFolderEdit->setToolTip(
            savedIncoming
        );
    }

    if (
        !savedWorkspace.isEmpty()
    )
    {
        workspaceFolderEdit->setText(
            savedWorkspace
        );

        workspaceFolderEdit->setToolTip(
            savedWorkspace
        );
    }
}


// ========================================================
// Save Settings
// ========================================================

void MainWindow::saveSettings()
{
    QSettings settings(
        "ProductPhotoAutomation",
        "ProductPhotoAutomation"
    );

    settings.setValue(
        "scalePort",
        serialPortCombo
            ->currentData()
            .toString()
    );

    settings.setValue(
        "incomingFolder",
        incomingFolderEdit->text()
    );

    settings.setValue(
        "workspaceFolder",
        workspaceFolderEdit->text()
    );

    settings.sync();
}


// ========================================================
// Reset Product
// ========================================================

void MainWindow::resetCurrentProduct()
{
    currentBarcode.clear();

    currentPhotoCount =
        0;

    cameraManager->clearCurrentBarcode();

    barcodeValueLabel->setText(
        "-"
    );

    photoCountLabel->setText(
        "0장"
    );
}


// ========================================================
// Scale Status
// ========================================================

void MainWindow::setScaleStatus(
    const QString &text,
    bool connected
)
{
    scaleStatusLabel->setText(
        text
    );

    if (
        connected
    )
    {
        scaleDotLabel->setStyleSheet(
            "color:#12B76A;"
        );

        scaleStatusLabel->setStyleSheet(
            "color:#027A48;"
        );
    }
    else
    {
        scaleDotLabel->setStyleSheet(
            "color:#B8C1CC;"
        );

        scaleStatusLabel->setStyleSheet(
            "color:#98A2B3;"
        );
    }

    // 스타일 변경 후 폰트 배율 다시 적용
    updateResponsiveUi();
}


// ========================================================
// Camera Status
// ========================================================

void MainWindow::setCameraStatus(
    const QString &text,
    bool active
)
{
    cameraStatusLabel->setText(
        text
    );

    if (
        active
    )
    {
        cameraDotLabel->setStyleSheet(
            "color:#12B76A;"
        );

        cameraStatusLabel->setStyleSheet(
            "color:#027A48;"
        );
    }
    else
    {
        cameraDotLabel->setStyleSheet(
            "color:#B8C1CC;"
        );

        cameraStatusLabel->setStyleSheet(
            "color:#98A2B3;"
        );
    }

    updateResponsiveUi();
}


// ========================================================
// Log
// ========================================================

void MainWindow::writeLog(
    const QString &message,
    const QString &type
)
{
    QString currentTime =
        QDateTime::currentDateTime()
            .toString(
                "HH:mm:ss"
            );

    QString tagColor =
        "#667085";

    QString textColor =
        "#344054";

    QString tagBackground =
        "#F2F4F7";

    QString normalizedType =
        type.toUpper();

    if (
        normalizedType == "DEVICE"
    )
    {
        tagColor =
            "#175CD3";

        tagBackground =
            "#EEF4FF";
    }
    else if (
        normalizedType == "CAMERA"
    )
    {
        tagColor =
            "#5925DC";

        tagBackground =
            "#F4F3FF";
    }
    else if (
        normalizedType == "BARCODE"
    )
    {
        tagColor =
            "#026AA2";

        tagBackground =
            "#F0F9FF";
    }
    else if (
        normalizedType == "PHOTO"
    )
    {
        tagColor =
            "#B54708";

        tagBackground =
            "#FFFAEB";
    }
    else if (
        normalizedType == "SAVE"
    )
    {
        tagColor =
            "#027A48";

        tagBackground =
            "#ECFDF3";
    }
    else if (
        normalizedType == "ERROR"
    )
    {
        tagColor =
            "#B42318";

        textColor =
            "#B42318";

        tagBackground =
            "#FEF3F2";
    }

    QString safeMessage =
        message.toHtmlEscaped();

    QString html =
        QString(
            "<div style='margin:3px 0;'>"
            "<span style='color:#98A2B3;'>%1</span>"
            "&nbsp;&nbsp;"
            "<span style='"
            "background:%2;"
            "color:%3;"
            "font-weight:700;"
            "padding:2px 5px;"
            "'>[%4]</span>"
            "&nbsp;&nbsp;"
            "<span style='color:%5;'>%6</span>"
            "</div>"
        )
            .arg(
                currentTime,
                tagBackground,
                tagColor,
                normalizedType,
                textColor,
                safeMessage
            );

    logBox->append(
        html
    );
}