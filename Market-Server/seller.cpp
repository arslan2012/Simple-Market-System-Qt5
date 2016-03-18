#include "seller.h"

Seller::Seller(QString _userName, QString _password, double _balance) :
    User(_userName,_password,_balance) {}

bool Seller::login(QString _userName, QString _password){
    if (_password == password && _userName == userName) {
        return true;
    }
    else {
        return false;
    }
}

void Seller::logout() {
    log = false;
}

QString Seller::getInformation() {
    return "Seller\nID:"+QString::number(id)+"\nUserName:"+userName+QString("\nBalance:%1").arg(balance);
}

void Seller::earning(double money) {
    balance += money;
}

int Seller::getID() {
    return id;
}

QString Seller::getName() {
    return userName;
}

QStringList Seller::getMyGoodsList(std::vector<Goods *> goods, QString str) {
    QStringList list;
    for (size_t i = 0; i < goods.size(); i++)
    {
        if (goods[i]->getOwner() == id)
        {
            if (str == "" || goods[i]->getName().contains(str))
            {
                list.append(goods[i]->getMessage());
            }
        }
    }
    return list;
}

QStringList Seller::getMyGoodsList(std::vector<Goods *> goods) {
    QStringList list;
    for (size_t i = 0; i < goods.size(); i++)
    {
        if (goods[i]->getOwner() == id)
        {
            list.append(goods[i]->getMessage());

        }
    }
    return list;
}

QStringList Seller::getGoodsList(std::vector<Goods *> goods)
{
    QStringList list;
    for (size_t i = 0; i < goods.size(); i++)
    {
        list.append(goods[i]->getMessage());
    }
    return list;
}

QStringList Seller::getGoodsList(std::vector<Goods *> goods,QString str)
{
    QStringList list;
    for (size_t i = 0; i < goods.size(); i++)
    {   if (str == "" || goods[i]->getName().contains(str))
        {
            list.append(goods[i]->getMessage());
        }
    }
    return list;
}


QDataStream &operator <<(QDataStream &out, const Seller &seller)
{
    QString type = "Seller";
    out << type << seller.id << seller.userName << seller.password << seller.balance;
    return out;
}

QDataStream &operator >>(QDataStream &in, Seller &seller)
{
    in >> seller.id >> seller.userName >> seller.password >> seller.balance;
    return in;
}
