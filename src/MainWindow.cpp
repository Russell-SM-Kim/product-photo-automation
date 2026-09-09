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

#include <QDateTime>
#include <QApplication>
#include <QFont>

#include <QFileDialog>
#include <QDir>

#include <QSettings>

#include <QSerialPortInfo>

MainWindow::MainWindow(
    QWidget *parent
)
    : QMainWindow(parent)
{
    setWindowTitle(
        "상품 촬영 자동화"
    );

    resize(
        850,
        750
    );

    // =========================================
    // Manager 생성
    // =========================================

    scaleManager =
        new ScaleManager(
            this
        );

    cameraManager =
        new CameraManager(
            this
        );

    // =========================================
    // 기본 UI
    // =========================================

    auto *centralWidget =
        new QWidget(this);

    setCentralWidget(
        centralWidget
    );

    auto *mainLayout =
        new QVBoxLayout(
            centralWidget
        );

    // =========================================
    // 제목
    // =========================================

    auto *titleLabel =
        new QLabel(
            "상품 촬영 자동화 시스템"
        );

    QFont titleFont;

    titleFont.setPointSize(
        18
    );

    titleFont.setBold(
        true
    );

    titleLabel->setFont(
        titleFont
    );

    mainLayout->addWidget(
        titleLabel
    );

    // =========================================
    // 환경 설정 제목
    // =========================================

    auto *settingsTitle =
        new QLabel(
            "환경 설정"
        );

    QFont settingsFont;

    settingsFont.setPointSize(
        13
    );

    settingsFont.setBold(
        true
    );

    settingsTitle->setFont(
        settingsFont
    );

    mainLayout->addWidget(
        settingsTitle
    );

    // =========================================
    // 환경 설정 Grid
    // =========================================

    auto *settingsLayout =
        new QGridLayout();

    // -----------------------------------------
    // COM Port
    // -----------------------------------------

    settingsLayout->addWidget(
        new QLabel(
            "전자저울 포트"
        ),
        0,
        0
    );

    serialPortCombo =
        new QComboBox();

    refreshPortsButton =
        new QPushButton(
            "새로고침"
        );

    settingsLayout->addWidget(
        serialPortCombo,
        0,
        1
    );

    settingsLayout->addWidget(
        refreshPortsButton,
        0,
        2
    );

    // -----------------------------------------
    // Incoming
    // -----------------------------------------

    settingsLayout->addWidget(
        new QLabel(
            "카메라 수신 폴더"
        ),
        1,
        0
    );

    incomingFolderEdit =
        new QLineEdit();

    incomingFolderEdit->setReadOnly(
        true
    );

    browseIncomingButton =
        new QPushButton(
            "찾기"
        );

    settingsLayout->addWidget(
        incomingFolderEdit,
        1,
        1
    );

    settingsLayout->addWidget(
        browseIncomingButton,
        1,
        2
    );

    // -----------------------------------------
    // Workspace
    // -----------------------------------------

    settingsLayout->addWidget(
        new QLabel(
            "작업 저장 폴더"
        ),
        2,
        0
    );

    workspaceFolderEdit =
        new QLineEdit();

    workspaceFolderEdit->setReadOnly(
        true
    );

    browseWorkspaceButton =
        new QPushButton(
            "찾기"
        );

    settingsLayout->addWidget(
        workspaceFolderEdit,
        2,
        1
    );

    settingsLayout->addWidget(
        browseWorkspaceButton,
        2,
        2
    );

    settingsLayout->setColumnStretch(
        1,
        1
    );

    mainLayout->addLayout(
        settingsLayout
    );

    // =========================================
    // 구분
    // =========================================

    auto *barcodeTitleLabel =
        new QLabel(
            "현재 바코드"
        );

    mainLayout->addWidget(
        barcodeTitleLabel
    );

    barcodeValueLabel =
        new QLabel(
            "-"
        );

    QFont barcodeFont;

    barcodeFont.setPointSize(
        16
    );

    barcodeFont.setBold(
        true
    );

    barcodeValueLabel->setFont(
        barcodeFont
    );

    mainLayout->addWidget(
        barcodeValueLabel
    );

    // =========================================
    // 저울
    // =========================================

    mainLayout->addWidget(
        new QLabel(
            "전자저울"
        )
    );

    scaleStatusLabel =
        new QLabel(
            "연결 안 됨"
        );

    mainLayout->addWidget(
        scaleStatusLabel
    );

    weightValueLabel =
        new QLabel(
            "현재 무게 : -"
        );

    QFont weightFont;

    weightFont.setPointSize(
        15
    );

    weightFont.setBold(
        true
    );

    weightValueLabel->setFont(
        weightFont
    );

    mainLayout->addWidget(
        weightValueLabel
    );

    // =========================================
    // Barcode Input
    // =========================================

    barcodeInput =
        new QLineEdit();

    barcodeInput->setPlaceholderText(
        "바코드를 스캔하세요"
    );

    mainLayout->addWidget(
        barcodeInput
    );

    // =========================================
    // 버튼
    // =========================================

    auto *buttonLayout =
        new QHBoxLayout();

    startButton =
        new QPushButton(
            "실행"
        );

    finishProductButton =
        new QPushButton(
            "상품 종료"
        );

    exitButton =
        new QPushButton(
            "프로그램 종료"
        );

    buttonLayout->addWidget(
        startButton
    );

    buttonLayout->addWidget(
        finishProductButton
    );

    buttonLayout->addWidget(
        exitButton
    );

    mainLayout->addLayout(
        buttonLayout
    );

    // =========================================
    // 로그
    // =========================================

    mainLayout->addWidget(
        new QLabel(
            "작업 로그"
        )
    );

    logBox =
        new QTextEdit();

    logBox->setReadOnly(
        true
    );

    mainLayout->addWidget(
        logBox
    );

    // =========================================
    // COM Port 새로고침
    // =========================================

    connect(
        refreshPortsButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            refreshSerialPorts();
        }
    );

    // =========================================
    // Incoming 폴더
    // =========================================

    connect(
        browseIncomingButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            selectIncomingFolder();
        }
    );

    // =========================================
    // Workspace 폴더
    // =========================================

    connect(
        browseWorkspaceButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            selectWorkspaceFolder();
        }
    );

    // =========================================
    // Barcode
    // =========================================

    connect(
        barcodeInput,
        &QLineEdit::returnPressed,
        this,
        &MainWindow::handleBarcode
    );

    // =========================================
    // 실행
    // =========================================

    connect(
        startButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            saveSettings();

            applyFolders();

            // ---------------------------------
            // COM Port
            // ---------------------------------

            if (
                serialPortCombo
                    ->currentIndex() < 0
            )
            {
                writeLog(
                    "전자저울 포트를 선택해주세요."
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
                    "전자저울 포트가 올바르지 않습니다."
                );

                return;
            }

            // ---------------------------------
            // Incoming
            // ---------------------------------

            if (
                incomingFolderEdit
                    ->text()
                    .trimmed()
                    .isEmpty()
            )
            {
                writeLog(
                    "카메라 수신 폴더를 선택해주세요."
                );

                return;
            }

            // ---------------------------------
            // Workspace
            // ---------------------------------

            if (
                workspaceFolderEdit
                    ->text()
                    .trimmed()
                    .isEmpty()
            )
            {
                writeLog(
                    "작업 저장 폴더를 선택해주세요."
                );

                return;
            }

            writeLog(
                "작업 시작"
            );

            // ---------------------------------
            // Scale
            // ---------------------------------

            if (
                !scaleManager
                     ->isConnected()
            )
            {
                writeLog(
                    QString(
                        "저울 %1 연결 시도"
                    )
                        .arg(
                            portName
                        )
                );

                scaleManager
                    ->connectScale(
                        portName
                    );
            }

            // ---------------------------------
            // Camera
            // ---------------------------------

            if (
                !cameraManager
                     ->isWatching()
            )
            {
                cameraManager
                    ->startWatching();
            }

            barcodeInput->setFocus();
        }
    );

    // =========================================
    // 상품 종료
    // =========================================

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
                    "종료할 상품이 없습니다."
                );

                return;
            }

            writeLog(
                QString(
                    "상품 종료 : %1"
                )
                    .arg(
                        currentBarcode
                    )
            );

            currentBarcode.clear();

            cameraManager
                ->clearCurrentBarcode();

            barcodeValueLabel
                ->setText(
                    "-"
                );

            barcodeInput
                ->clear();

            barcodeInput
                ->setFocus();
        }
    );

    // =========================================
    // 종료
    // =========================================

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

    // =========================================
    // Scale Signals
    // =========================================

    connect(
        scaleManager,
        &ScaleManager::connected,
        this,
        [this](
            const QString &portName
        )
        {
            scaleStatusLabel
                ->setText(
                    QString(
                        "연결됨 (%1)"
                    )
                        .arg(
                            portName
                        )
                );

            writeLog(
                QString(
                    "전자저울 연결 성공 : %1"
                )
                    .arg(
                        portName
                    )
            );
        }
    );

    connect(
        scaleManager,
        &ScaleManager::disconnected,
        this,
        [this]()
        {
            scaleStatusLabel
                ->setText(
                    "연결 안 됨"
                );

            weightValueLabel
                ->setText(
                    "현재 무게 : -"
                );

            hasWeight =
                false;

            writeLog(
                "전자저울 연결 종료"
            );
        }
    );

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

            weightValueLabel
                ->setText(
                    QString(
                        "현재 무게 : %1 kg"
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

    connect(
        scaleManager,
        &ScaleManager::errorOccurred,
        this,
        [this](
            const QString &message
        )
        {
            scaleStatusLabel
                ->setText(
                    "연결 오류"
                );

            writeLog(
                message
            );
        }
    );

    // =========================================
    // Camera Signals
    // =========================================

    connect(
        cameraManager,
        &CameraManager::watchingStarted,
        this,
        [this](
            const QString &folderPath
        )
        {
            writeLog(
                QString(
                    "카메라 폴더 감시 시작 : %1"
                )
                    .arg(
                        folderPath
                    )
            );
        }
    );

    connect(
        cameraManager,
        &CameraManager::watchingStopped,
        this,
        [this]()
        {
            writeLog(
                "카메라 폴더 감시 종료"
            );
        }
    );

    connect(
        cameraManager,
        &CameraManager::photoDetected,
        this,
        [this](
            const QString &filePath
        )
        {
            writeLog(
                QString(
                    "사진 감지 : %1"
                )
                    .arg(
                        filePath
                    )
            );
        }
    );

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
            writeLog(
                QString(
                    "사진 저장 완료 : %1_%2"
                )
                    .arg(
                        barcode
                    )
                    .arg(
                        photoIndex
                    )
            );

            writeLog(
                QString(
                    "저장 경로 : %1"
                )
                    .arg(
                        savedPath
                    )
            );
        }
    );

    connect(
        cameraManager,
        &CameraManager::errorOccurred,
        this,
        [this](
            const QString &message
        )
        {
            writeLog(
                QString(
                    "카메라 오류 : %1"
                )
                    .arg(
                        message
                    )
            );
        }
    );

    // =========================================
    // 초기 설정 로드
    // =========================================

    refreshSerialPorts();

    loadSettings();

    applyFolders();

    writeLog(
        "프로그램 실행"
    );

    barcodeInput->setFocus();
}

// =============================================
// Barcode
// =============================================

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

    if (
        !currentBarcode.isEmpty() &&
        currentBarcode != barcode
    )
    {
        writeLog(
            QString(
                "이전 상품 자동 종료 : %1"
            )
                .arg(
                    currentBarcode
                )
        );
    }

    currentBarcode =
        barcode;

    barcodeValueLabel
        ->setText(
            currentBarcode
        );

    cameraManager
        ->setCurrentBarcode(
            currentBarcode
        );

    if (hasWeight)
    {
        writeLog(
            QString(
                "바코드 인식 : %1 / 무게 : %2 kg"
            )
                .arg(
                    currentBarcode
                )
                .arg(
                    currentWeight,
                    0,
                    'f',
                    2
                )
        );

        QString errorMessage;

        bool saved =
            FileManager
                ::saveProductData(
                    currentBarcode,
                    currentWeight,
                    &errorMessage
                );

        if (saved)
        {
            writeLog(
                QString(
                    "상품 데이터 저장 완료 : %1"
                )
                    .arg(
                        currentBarcode
                    )
            );
        }
        else
        {
            writeLog(
                QString(
                    "상품 데이터 저장 실패 : %1"
                )
                    .arg(
                        errorMessage
                    )
            );
        }
    }
    else
    {
        writeLog(
            QString(
                "바코드 인식 : %1 / 무게 없음"
            )
                .arg(
                    currentBarcode
                )
        );
    }

    barcodeInput->clear();

    barcodeInput->setFocus();
}

// =============================================
// Serial Port 검색
// =============================================

void MainWindow::refreshSerialPorts()
{
    QString previousPort =
        serialPortCombo
            ->currentData()
            .toString();

    serialPortCombo
        ->clear();

    const QList<QSerialPortInfo> ports =
        QSerialPortInfo
            ::availablePorts();

    for (
        const QSerialPortInfo &port :
        ports
    )
    {
        QString displayName =
            port.portName();

        if (
            !port.description()
                 .isEmpty()
        )
        {
            displayName +=
                " - " +
                port.description();
        }

        serialPortCombo
            ->addItem(
                displayName,
                port.portName()
            );
    }

    if (
        serialPortCombo->count() == 0
    )
    {
        serialPortCombo
            ->addItem(
                "사용 가능한 COM 포트 없음",
                ""
            );

        return;
    }

    // 기존 선택 복원
    if (
        !previousPort.isEmpty()
    )
    {
        int index =
            serialPortCombo
                ->findData(
                    previousPort
                );

        if (index >= 0)
        {
            serialPortCombo
                ->setCurrentIndex(
                    index
                );
        }
    }
}

// =============================================
// Incoming 폴더 선택
// =============================================

void MainWindow::selectIncomingFolder()
{
    QString initialFolder =
        incomingFolderEdit
            ->text();

    QString folder =
        QFileDialog
            ::getExistingDirectory(
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

    incomingFolderEdit
        ->setText(
            QDir::toNativeSeparators(
                folder
            )
        );

    applyFolders();

    saveSettings();
}

// =============================================
// Workspace 선택
// =============================================

void MainWindow::selectWorkspaceFolder()
{
    QString initialFolder =
        workspaceFolderEdit
            ->text();

    QString folder =
        QFileDialog
            ::getExistingDirectory(
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

    workspaceFolderEdit
        ->setText(
            QDir::toNativeSeparators(
                folder
            )
        );

    applyFolders();

    saveSettings();
}

// =============================================
// 설정 적용
// =============================================

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
        cameraManager
            ->setIncomingFolder(
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

        cameraManager
            ->setProductsFolder(
                productsFolder
            );

        FileManager
            ::setDataFolder(
                dataFolder
            );
    }
}

// =============================================
// Settings Load
// =============================================

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
            serialPortCombo
                ->findData(
                    savedPort
                );

        if (index >= 0)
        {
            serialPortCombo
                ->setCurrentIndex(
                    index
                );
        }
    }

    if (
        !savedIncoming.isEmpty()
    )
    {
        incomingFolderEdit
            ->setText(
                savedIncoming
            );
    }

    if (
        !savedWorkspace.isEmpty()
    )
    {
        workspaceFolderEdit
            ->setText(
                savedWorkspace
            );
    }
}

// =============================================
// Settings Save
// =============================================

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
        incomingFolderEdit
            ->text()
    );

    settings.setValue(
        "workspaceFolder",
        workspaceFolderEdit
            ->text()
    );

    settings.sync();
}

// =============================================
// Log
// =============================================

void MainWindow::writeLog(
    const QString &message
)
{
    QString currentTime =
        QDateTime
            ::currentDateTime()
            .toString(
                "yyyy-MM-dd HH:mm:ss"
            );

    logBox
        ->append(
            QString(
                "[%1] %2"
            )
                .arg(
                    currentTime,
                    message
                )
        );
}