#ifndef USER_H
#define USER_H
#include<QStringList>
#include<QDataStream>
#include<goods.h>

/*User*/
class User{
public:
    User(QString _userName = "",QString _password = "",double _balance = 0.0);

    virtual QString getInformation();               //查看用户个人信息
    QStringList getGoodsList(std::vector<Goods *> goods);
    QStringList getGoodsList(std::vector<Goods *> goods, QString str);

    bool login(QString _userName, QString _password);
    void logout();
    static int count;
protected:
    bool log;
    int id;
    QString userName;
    QString password;
    double balance;
};


#endif // USER_H
