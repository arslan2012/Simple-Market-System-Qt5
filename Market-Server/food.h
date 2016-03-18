#ifndef FOOD_H
#define FOOD_H

#include <goods.h>

/*Food*/

class Food:protected Goods{
public:
    Food();
    Food(int _owner, QString _name, int _amount, double _price, double _discount, \
         QDate _manuDate, QDate _disDate, QDate _validDate);

    double getPrice();
    virtual QString getMessage();
    virtual QString getType(){return "Food";}

    friend QDataStream &operator<< (QDataStream &out, const Food &food);
    friend QDataStream &operator>> (QDataStream &in, Food &food);
protected:
    QDate manuDate;
    QDate discDate;
    QDate validDate;
    double discount;
};

#endif // FOOD_H
