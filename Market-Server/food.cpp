#include "food.h"

/*Foods*/
Food::Food() :
    Goods(FOOD)
{
    discount = 1.0;
    manuDate = QDate::currentDate();
    discDate = QDate::currentDate();
    validDate = QDate::currentDate();

}

Food::Food(int _owner, QString _name, int _amount, double _price, double _discount, QDate _manuDate, QDate _disDate, QDate _validDate) :
    Goods(FOOD,_owner,_name,_amount,_price)
{
    discount = _discount;
    manuDate = _manuDate;
    discDate = _disDate;
    validDate = _validDate;
}

double Food::getPrice() {
    QDate currentDate = QDate::currentDate();
    if (currentDate > validDate) {
        return -1;
    } else if (currentDate > discDate) {
        return price * discount;
    }
    else {
        return price;
    }
}
QString Food::getMessage() {
    return "商品名:"+name+" 商品id:"+QString::number(id)+" 卖家:"+QString::number(owner)+"  库存:"+QString::number(amount)+QString("  价格: %1").arg(price)\
            +"  生产日期: "+manuDate.toString("yyyy/MM/dd")+"  有效期: "+validDate.toString("yyyy/MM/dd");
}

QDataStream &operator<< (QDataStream &out, const Food &food)
{
    QString type = "Food";
    out << type << food.id << food.owner << food.name << food.amount << food.price << food.discount <<
           food.manuDate.toString("yyyy/MM/dd") << food.discDate.toString("yyyy/MM/dd") << food.validDate.toString("yyyy/MM/dd");
    return out;
}

QDataStream &operator>> (QDataStream &in, Food &food)
{
    QString mDate,discDate,vDate;
    in >> food.id >> food.owner >> food.name >> food.amount >> food.price >> food.discount >> mDate >> discDate >> vDate;
    food.manuDate.fromString(mDate,"yyyy/MM/dd");
    food.discDate.fromString(discDate,"yyyy/MM/dd");
    food.validDate.fromString(vDate,"yyyy/MM/dd");

    return in;
}

