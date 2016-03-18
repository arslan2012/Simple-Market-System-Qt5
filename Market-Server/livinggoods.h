#ifndef LIVINGGOODS_H
#define LIVINGGOODS_H

#include <goods.h>

class LivingGoods:protected Goods{
public:
    LivingGoods();
    LivingGoods(int _owener,QString _name, int _amount, double _price, QDate _manuDate, QDate _validDate);

    double getPrice();
    virtual QString getMessage();
    virtual QString getType(){return "LivingGoods";}

    friend QDataStream &operator<< (QDataStream &out, const LivingGoods &live);
    friend QDataStream &operator>> (QDataStream &in, LivingGoods &live);
protected:
    QDate manuDate;
    QDate validDate;
};

#endif // LIVINGGOODS_H
