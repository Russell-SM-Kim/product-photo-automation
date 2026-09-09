#pragma once

#include <QObject>
#include <QSerialPort>

class ScaleManager : public QObject
{
    Q_OBJECT

public:
    explicit ScaleManager(QObject *parent = nullptr);

    bool connectScale(const QString &portName);
    void disconnectScale();
    bool isConnected() const;

signals:
    void connected(const QString &portName);
    void disconnected();

    void weightChanged(double weight);

    void errorOccurred(const QString &message);

private slots:
    void handleReadyRead();
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serialPort;

    QByteArray receiveBuffer;

    double lastWeight = -999999.0;
};