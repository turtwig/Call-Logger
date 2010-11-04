#include "MainWindow.h"

#include <QTableWidget>
#include <QStringList>
#include <QTcpSocket>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), host("192.168.10.151"), port(4001) {
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
    while(!s.atEnd()) {
        QStringList data = s.readLine().split(",");
        QStringList data2 = data[0].split(" ");
        if (data.length() < 10 || data2.length() < 2) {
            QMessageBox b;
            b.setIcon(QMessageBox::Information);
            b.setText(tr("Received malformed string."));
            b.setInformativeText(tr("The line was ignored."));
            b.exec();
            continue;
        }
        int r = table->rowCount();
        table->setRowCount(table->rowCount()+1);
        QColor bg;
        if (data[4].length() > 4) {
            bg = (r % 2) == 0 ? QColor(227, 242, 247) : QColor(214, 236, 244); // incoming
        } else {
            bg = (r % 2) == 0 ? QColor(227, 241, 226) : QColor(214, 234, 214); // outgoing
        }
        QTableWidgetItem* i = new QTableWidgetItem(data2[0]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 0, i);
        i = new QTableWidgetItem(data2[1]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 1, i);
        i = new QTableWidgetItem(data[1]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 2, i);
        i = new QTableWidgetItem(data[3]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 3, i);
        i = new QTableWidgetItem(data[4]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 4, i);
        i = new QTableWidgetItem(data[5]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 5, i);
        i = new QTableWidgetItem(data[6]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 6, i);
        i = new QTableWidgetItem(data[7]);
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 7, i);
        long ms = data[8].toInt();
        long mins = (ms/1000)/60;
        long secs = ms/1000-(mins*60);
        QString s = mins < 10 ? tr("0%1") : tr("%1");
        s += secs < 10 ? tr(":0%2") : tr(":%2");
        i = new QTableWidgetItem(s.arg(mins).arg(secs));
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 8, i);
        ms = data[9].toInt();
        mins = (ms/1000)/60;
        secs = ms/1000-(mins*60);
        s = mins < 10 ? tr("0%1") : tr("%1");
        s += secs < 10 ? tr(":0%2") : tr(":%2");
        i = new QTableWidgetItem(s.arg(mins).arg(secs));
        i->setFlags(Qt::ItemIsEnabled);
        i->setBackgroundColor(bg);
        table->setItem(r, 9, i);
        table->setCurrentItem(i);
        table->setCurrentItem(0);
    }
}
