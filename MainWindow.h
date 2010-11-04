#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>

class QTableWidget;
class QTcpSocket;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readData();
    void handshake();
    void sockErr();

private:
    QTableWidget* table;
    QTcpSocket* sock;
    const QString host;
    const int port;
};

#endif // MAINWINDOW_H