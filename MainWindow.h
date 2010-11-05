#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QHeaderView>

class QTableWidget;
class QTcpSocket;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void readData();
    void handshake();
    void sockErr();

private:
    QTableWidget* table;
    QTcpSocket* sock;
    QHeaderView* hView;
    QHeaderView* vView;
    const QString host;
    const int port;
};

#endif // MAINWINDOW_H
