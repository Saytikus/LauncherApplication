#include "AppThread.h"

AppThread::AppThread(QObject *parent) : QObject{parent} {

}

void AppThread::SendFromBufferContinue(const QStringList address_port_list) {
    qDebug() << "Continue start";
    QHostAddress addr;
    addr.setAddress(address_port_list[0]);
    quint16 port = address_port_list[1].toUInt();
    this->SendFromBufferComplete(send_data_, addr, port);
    send_data_.clear();
}

void AppThread::SendFromBufferStart(const QBuffer* buffer, const QString buffer_id) {
    send_data_ = buffer->data();
    qDebug() << "Поток в SendFromBufferStart: " << QThread::currentThread(); //
    emit IdDecompositionCall(buffer_id, true);
    emit HandleSendBufferFinished(buffer_id);
}

void AppThread::SendFromBufferComplete(const QByteArray send_data, const QHostAddress client_address, const quint16 client_port) {
    qDebug() << "Информация перед отправкой: " << send_data;
    qDebug() << "Поток в SendFromBufferComplete: " << QThread::currentThread();
    emit SendCall(send_data, client_address, client_port);
}
