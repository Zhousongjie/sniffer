#ifndef MYSNIFFER_H
#define MYSNIFFER_H

#include <QThread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "packetstruct.h"

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)


class MySniffer : public QThread
{
    Q_OBJECT
public:
    explicit MySniffer(QObject *parent,QString ip ,int count);

    void setIp(QString ip);
    void setCount(int count);

    bool threadStop = false;

    void run();

signals:
    void sendData(QStringList data);
    void sendError(QString data);

public slots:
    void stopThread();


private:
     QString ip;
     int count = 0;

     const DWORD dwBufSize = 10240;
     QString getDataFromPacket(BYTE *lpBuf, int iLen, int iPrintType);
     void analyseRecvPacket(BYTE *lpBuf);

};

#endif // MYSNIFFER_H
