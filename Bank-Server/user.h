#ifndef USER_H
#define USER_H
#include<QDataStream>

class user
{
public:
    user(QString _idNum = "",QString _password = "",quint8 _bankChoice = 0,int _cardNum = 0);
    QString getInformation();
    bool login(QString _idNum = "",QString _password = "");
    void logout();
    void chargeBalance(double number = 0.0);
    bool withdraw(double number = 0.0);
    int getCardNum(void){return cardNum;}
    QString getId(){return idNum;}
    void setPwd(QString pwd = ""){password=pwd;}
    friend QDataStream &operator<<(QDataStream &out, const user &userStream);
    friend QDataStream &operator>>(QDataStream &in, user &userStream);
protected:
    bool log;
    QString idNum;
    QString password;
    double balance;
    int cardNum;
    quint8 bankChoice;
};

#endif // USER_H
