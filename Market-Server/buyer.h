#ifndef BUYER_H
#define BUYER_H

#include <user.h>

class Buyer:protected User{
public:
    Buyer(QString _userName = "",QString _password ="",double _balance = 0.0);

    void chargeBalance(double number);
    bool login(QString _userName,QString _password); //log in
    void logout();

    virtual QString getInformation();
    virtual int purchase(double price);
    QStringList getGoodsList(std::vector<Goods *> goods);
    QStringList getGoodsList(std::vector<Goods *> goods,QString str);
    QString getName();
    double getBalance(){return balance;}

    friend QDataStream &operator<<(QDataStream &out,const Buyer &buyer);
    friend QDataStream &operator>>(QDataStream &in,Buyer &buyer);
    std::vector<int> cartId;
    std::vector<int> cartAmount;
    std::vector<int> cardNum;
protected:
};

#endif // BUYER_H
