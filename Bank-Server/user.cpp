#include "user.h"

user::user(QString _idNum,QString _password, quint8 _bankChoice,int _cardNum){
    log = false;
    idNum = _idNum;
    password = _password;
    balance = 0;
    bankChoice = _bankChoice;
    cardNum = _cardNum;
}
bool user::login(QString _idNum,QString _password) {
    if (idNum == _idNum && _password == password) {
        log = true;
        return true;
    } else {
        return false;
    }
}
void user::logout() {
    log = false;
}
QString user::getInformation() {
    return "ID Number:  "+idNum+"\nCard Num:  "+QString::number(cardNum)+QString(" \nBalance: %1").arg(balance);
}
void user::chargeBalance(double number) {  //to  do
    balance += number;
}
bool user::withdraw(double number) {  //to  do
    if(number<=balance){
    balance -= number;
    return true;
    }else return false;
}
QDataStream &operator<<(QDataStream &out, const user &userStream)
{
    QString type = "true";
    out << type << userStream.idNum << userStream.password << userStream.balance << userStream.cardNum << userStream.bankChoice;
    return out;
}

QDataStream &operator>>(QDataStream &in, user &userStream)
{
    in >> userStream.idNum >> userStream.password >> userStream.balance >> userStream.cardNum >> userStream.bankChoice;
    return in;
}
