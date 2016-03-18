#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QDataStream>
#include <QFile>
#include <QMap>
#include <QMessageBox>

#include "user.h"
enum messagetypes{LOGIN,SIGNUP,INFO,LOGOUT,CHARGE,WITHDRAW,GETBANK,CHANGEPWD,BINDCARD,CARDWITHDRAW};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void read_file();
    int generateCard(quint8 _bankChoice = 0);
private:
    Ui::MainWindow *ui;
    QFile fout,fin;
    std::vector<user *> users;
    QMap<QString,int> userRcd;
    QUdpSocket *udpSocket;
private slots:
    void processPendingDatagrams();
};

#endif // MAINWINDOW_H
