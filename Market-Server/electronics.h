#ifndef ELECTRONICS_H
#define ELECTRONICS_H
#include <goods.h>

class Electronics:protected Goods{
public:
    Electronics();
    Electronics(int _owner,QString name, int _amount, double _price, QDate _manuDate, QDate _validDate);

    double getPrice();
    virtual QString getMessage();
    virtual QString getType(){return "Electronics";}

    friend QDataStream &operator<<(QDataStream &out, const Electronics &elet);
    friend QDataStream &operator>>(QDataStream &in, Electronics &elet);
protected:
    QDate manuDate;
    QDate validDate;
    double discount;
};

#endif // ELECTRONICS_H
