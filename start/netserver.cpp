#include "netserver.h"

netServer::netServer()
{

}


void netServer::readPendingDatagrams()
{
    while (receiver->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(), datagram.size());
        //���ݽ�����datagram��
            /* readDatagram ����ԭ��
            qint64 readDatagram(char *data,qint64 maxSize,QHostAddress *address=0,quint16 *port=0)
        */
    }
}