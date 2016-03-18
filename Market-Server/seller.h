#ifndef SELLER_H
#define SELLER_H

#include<user.h>

class Seller:protected User {
public:
    Seller(QString _userName = "",QString _password = "",double _balance = 0.0);

    void stock();
    bool login(QString _userName,QString _password); //log in
    void logout();
    virtual QString getInformation();

    QStringList getGoodsList(std::vector<Goods *> goods);
    QStringList getGoodsList(std::vector<Goods *> goods, QString str);
    QStringList getMyGoodsList(std::vector<Goods *> goods);
    QStringList getMyGoodsList(std::vector<Goods *> goods, QString str);
    void earning(double money);
    QString getName();
    int getID();

    friend QDataStream &operator<<(QDataStream &out,const Seller &seller);
    friend QDataStream &operator>>(QDataStream &in,Seller &seller);
protected:
};

#endif // SELLER_H
