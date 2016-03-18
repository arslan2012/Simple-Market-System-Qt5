#ifndef GOODS_H
#define GOODS_H
#define FOOD 1
#define ETRO 2
#define LIVE 3
#include <QDate>
#include <QDataStream>

/*Goods*/
class Goods {
public:
    Goods(int _type = 0, int _owner = 0, QString _name = "",int _amount = 0,double _price = 0.0);

    virtual double getPrice() = 0;
    virtual QString getMessage();
    virtual QString getType(){return "Goods";}
    void decAmount(int num);
    int getAmount();
    int getOwner();
    int getId();
    QString getName();
    static int count;
    int type;
protected:
    int id;
    QString name;
    int amount;
    double price;
    int owner;  //owner实际为user的ID
};

#endif // GOODS_H
