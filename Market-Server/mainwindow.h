#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QDataStream>
#include <QFile>
#include <QMap>
#include <QMessageBox>

#include<buyer.h>
#include<seller.h>
#include<food.h>
#include <electronics.h>
#include<livinggoods.h>

enum messageType{LOGOUT,LOGIN,SIGNIN,INFO,GOODSLIST,GETGOOD,CARDAMOUNT,CHARGE,SEARCH,MYGOODSLIST,MYSEARCH,STOCK,PURCHASE,GETCART,ADDCART,BINDCARD};
enum BankMessage{TESTCARD = 8,CARDWITHDRAW = 9};
enum userType{UNLOG,BUYER,SELLER};

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
    void read_disfile();
    void write_file();
    void del_item();
    bool purchase(int pos, int id, int amount);
private:
    Ui::MainWindow *ui;
    
    QFile fout,fin;
    std::vector<Buyer *> buyers;
    std::vector<Seller *> sellers;
    std::vector<Goods *> goods;

    QMap<QString,int> BuyerRcd;
    QMap<QString,int> SellerRcd;


    QUdpSocket *udpClientSocket;
    QUdpSocket *udpBankSocket;
    QHostAddress host;
    QHostAddress bankhost;
    quint16 port;

    double eledis;
    double fooddis;
    double livedis;
    int disamount;
    double amountdis;

private slots:
    void processPendingDatagrams();

};

#endif // MAINWINDOW_H
