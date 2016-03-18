#include "buyer.h"

/******        Buyer            ******/

Buyer::Buyer(QString _userName, QString _password, double _balance):
    User(_userName,_password,_balance) {}

void Buyer::chargeBalance(double number) {
    balance += number;
}

bool Buyer::login(QString _userName, QString _password){
    if (_password == password && _userName == userName) {
        return true;
    }
    else {
        return false;
    }
}

void Buyer::logout() {
    log = false;
}

QString Buyer::getName() {
    return userName;
}

QString Buyer::getInformation() {
    return "Buyer\nID:"+QString::number(id)+"\nUserName:"+userName+QString("\nBalance: %1").arg(balance);
}

int Buyer::purchase(double price) {
    if (abs(price + 1) < 1e-5) //
        return -1;
    else if (price < balance) {
        balance -= price;
        return 0;
    }
    else {
        return 1;
    }
}

QStringList Buyer::getGoodsList(std::vector<Goods *> goods)
{
    QStringList list;
    for (size_t i = 0; i < goods.size(); i++)
    {
        list.append(goods[i]->getMessage());

    }
    return list;
}

QStringList Buyer::getGoodsList(std::vector<Goods *> goods,QString str)
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

QDataStream &operator << (QDataStream &out, const Buyer &buyer)
{
    QString type = "Buyer";
    out << type << buyer.id << buyer.userName << buyer.password << buyer.balance;
    int size = buyer.cartId.size();
    out << size;
    for (size_t i = 0; i < buyer.cartId.size(); i++){
            out << buyer.cartId[i];
    }
    size = buyer.cartAmount.size();
    out << size;
    for (size_t i = 0; i < buyer.cartAmount.size(); i++){
            out << buyer.cartAmount[i];
    }
    size = buyer.cardNum.size();
    out << size;
    for (size_t i = 0; i < buyer.cartAmount.size(); i++){
            out << buyer.cardNum[i];
    }
    return out;
}

QDataStream &operator >>(QDataStream &in, Buyer &buyer)
{
    in >> buyer.id >> buyer.userName >> buyer.password >> buyer.balance;
    int size;
    in >> size;
    int n;
    for (int i = 0; i < size; i++){
            in >> n;
            buyer.cartId.push_back(n);
    }
    in >> size;
    for (int i = 0; i < size; i++){
            in >> n;
            buyer.cartAmount.push_back(n);
    }
    in >> size;
    for (int i = 0; i < size; i++){
            in >> n;
            buyer.cardNum.push_back(n);
    }
    return in;
}
