#include "MainWindow.h"

#include <QTableWidget>
#include <QStringList>
#include <QTcpSocket>
#include <QTextStream>
#include <QMessageBox>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), /**/host("192.168.10.151"),/*host("127.0.0.1"),*/ port(4001) {
    sock = new QTcpSocket(this);
    connect(sock, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(sock, SIGNAL(connected()), this, SLOT(handshake()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sockErr()));
    sock->connectToHost(host, port);

    table = new QTableWidget(0, 10, this);
    QStringList headers;
    headers << "Date" << "Time" << "To" << "Who" << "From" << "Answered" << "Transferred" << "Dialed" << "Duration" << "Call Time";
    table->setHorizontalHeaderLabels(headers);
    setCentralWidget(table);
}

MainWindow::~MainWindow() {
}

void MainWindow::sockErr() {
    QMessageBox b;
    b.setIcon(QMessageBox::Critical);
    b.setText(tr("Error: %1").arg(sock->errorString()));
    b.setInformativeText(tr("Do you want to reconnect?"));
    b.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (b.exec() == QMessageBox::Yes) {
        sock->connectToHost(host, port);
    } else {
        b.setIcon(QMessageBox::Information);
        b.setText(tr("Disconnected"));
        b.setInformativeText(tr("This application will not retry to connect."));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
    }
}

void MainWindow::handshake() {
    QTextStream s(sock);
    s << "\r\n";
}

void MainWindow::readData() {
    QTextStream s(sock);
    QString str;
    s >> str;
    qDebug() << "Received:" << str;
}
