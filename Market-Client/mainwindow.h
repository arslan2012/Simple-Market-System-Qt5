#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include <QDataStream>
#include <QStandardItemModel>
#include <QtNetwork/QUdpSocket>

enum messageType{LOGOUT,LOGIN,SIGNIN,INFO,GOODSLIST,GETGOOD,CARDAMOUNT,CHARGE,SEARCH,MYGOODSLIST,MYSEARCH,STOCK,PURCHASE,GETCART,ADDCART,BINDCARD};
enum userType{UNLOG,BUYER,SELLER};

#define FOOD 1
#define ETRO 2
#define LIVE 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void goods_display(QStringList strList);

private slots:
    void on_login_loginBtn_clicked();

    void on_returnBtn_clicked();

    void on_goodsList_searchBtn_clicked();

    void on_getInfoBtn_clicked();

    void on_logoutBtn_clicked();

    void on_goodsListBtn_clicked();

    void on_returnBtn_2_clicked();

    void on_returnBtn_3_clicked();

    void on_chargeBtn_clicked();

    void on_logoutBtn_2_clicked();

    void on_getInfoBtn_2_clicked();

    void on_goodsListBtn_2_clicked();

    void on_myGoodsListBtn_clicked();

    void on_login_signinBtn_clicked();

    void on_charge_chargeBtn_clicked();

    void on_returnBtn_4_clicked();

    void on_stockBtn_clicked();

    void on_food_rBtn_clicked();

    void on_elet_rBtn_clicked();

    void on_living_rBtn_clicked();

    void on_stoke_stokeBtn_clicked();

    void on_mDateEdit_dateChanged(const QDate &date);

    void on_vDateEdit_dateChanged(const QDate &date);

    void on_disDateEdit_dateChanged(const QDate &date);

    void on_bind_bindBtn_clicked();

    void on_bindButton_clicked();

    void on_pushButton_clicked();

    void on_goodsList_addBtn_clicked();

    void on_cartBtn_clicked();

    void on_purchaseBtn_clicked();

    void on_returnBtn_5_clicked();

    void on_cartList_reAddBtn_clicked();

    void on_goodsList_changeBtn_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *standardItemModel;

    QUdpSocket *udpSocket;
    int func;

    QHostAddress host;
    quint16 port;

    int islogin;
};

#endif // MAINWINDOW_H
