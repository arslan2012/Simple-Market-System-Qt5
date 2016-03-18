#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QMessageBox>

enum messagetypes{LOGIN,SIGNUP,INFO,LOGOUT,CHARGE,WITHDRAW,GETBANKS,CHANGEPWD};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_login_loginBtn_clicked();

    void on_login_signinBtn_clicked();

    void on_returnBtn_clicked();

    void on_chargeBtn_clicked();

    void on_getInfoBtn_clicked();

    void on_logoutBtn_clicked();

    void on_withdrawBtn_clicked();

    void on_signup_signinBtn_clicked();

    void on_returnBtn_3_clicked();

    void on_returnBtn_2_clicked();

    void on_charge_chargeBtn_clicked();

    void on_withdraw_withdrawBtn_clicked();

    void on_PwdBtn_clicked();

    void on_pwd_pwdBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    QHostAddress host;
    quint16 port;
    bool islogin;
};

#endif // MAINWINDOW_H
