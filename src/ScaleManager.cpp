#include "ScaleManager.h"

#include <QRegularExpression>
#include <QtMath>

ScaleManager::ScaleManager(QObject *parent)
    : QObject(parent),
      serialPort(new QSerialPort(this))
{
    connect(
        serialPort,
        &QSerialPort::readyRead,
        this,
        &ScaleManager::handleReadyRead
    );

    connect(
        serialPort,
        &QSerialPort::errorOccurred,
        this,
        &ScaleManager::handleSerialError
    );
}

bool ScaleManager::connectScale(const QString &portName)
{
    if (serialPort->isOpen())
    {
        serialPort->close();
    }

    serialPort->setPortName(portName);

    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadOnly))
    {
        emit errorOccurred(
            QString("저울 연결 실패 (%1) : %2")
                .arg(
                    portName,
                    serialPort->errorString()
                )
        );

        return false;
    }

    receiveBuffer.clear();

    emit connected(portName);

    return true;
}

void ScaleManager::disconnectScale()
{
    if (!serialPort->isOpen())
    {
        return;
    }

    serialPort->close();

    emit disconnected();
}

bool ScaleManager::isConnected() const
{
    return serialPort->isOpen();
}

void ScaleManager::handleReadyRead()
{
    receiveBuffer.append(
        serialPort->readAll()
    );

    QString text =
        QString::fromLatin1(receiveBuffer);

    /*
        저울 데이터 특성

        숫자 자릿수가 역순으로 들어옴.

        예)
        실제 0.31 kg
        RAW  =13.00000
        → 13 뒤집기 → 31
        → 0.31 kg

        실제 0.43 kg
        RAW  =34.00000
        → 34 뒤집기 → 43
        → 0.43 kg

        실제 12 kg
        RAW에서 21 형태
        → 뒤집기 → 12 kg
    */

    QRegularExpression regex(
        R"(=(\d+)\.(\d+))"
    );

    QRegularExpressionMatchIterator iterator =
        regex.globalMatch(text);

    bool found = false;
    double newestWeight = 0.0;

    while (iterator.hasNext())
    {
        QRegularExpressionMatch match =
            iterator.next();

        QString beforeDot =
            match.captured(1);

        QString afterDot =
            match.captured(2);

        // ---------------------------------
        // 자릿수 뒤집기
        // ---------------------------------

        std::reverse(
            beforeDot.begin(),
            beforeDot.end()
        );

        // ---------------------------------
        // 소수점 뒤쪽에서 0 제거
        // ---------------------------------

        while (
            afterDot.endsWith('0') &&
            afterDot.length() > 1
        )
        {
            afterDot.chop(1);
        }

        // 소수점 뒤 값도 자릿수 뒤집기
        std::reverse(
            afterDot.begin(),
            afterDot.end()
        );

        bool beforeOk = false;
        bool afterOk = false;

        int decimalValue =
            beforeDot.toInt(
                &beforeOk
            );

        int integerValue =
            afterDot.toInt(
                &afterOk
            );

        if (
            beforeOk &&
            afterOk
        )
        {
            newestWeight =
                static_cast<double>(
                    integerValue
                )
                +
                static_cast<double>(
                    decimalValue
                ) / 100.0;

            found = true;
        }
    }

    if (found)
    {
        if (
            qAbs(
                newestWeight -
                lastWeight
            ) >= 0.001
        )
        {
            lastWeight =
                newestWeight;

            emit weightChanged(
                newestWeight
            );
        }
    }

    // 버퍼 무한 증가 방지
    if (
        receiveBuffer.size() > 256
    )
    {
        receiveBuffer =
            receiveBuffer.right(
                128
            );
    }
}

void ScaleManager::handleSerialError(
    QSerialPort::SerialPortError error
)
{
    if (error == QSerialPort::NoError)
    {
        return;
    }

    if (
        error == QSerialPort::ResourceError ||
        error == QSerialPort::DeviceNotFoundError ||
        error == QSerialPort::PermissionError
    )
    {
        emit errorOccurred(
            QString("시리얼 오류 : %1")
                .arg(
                    serialPort->errorString()
                )
        );
    }
}