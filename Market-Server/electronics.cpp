#include "electronics.h"

Electronics::Electronics():
    Goods(ETRO)
{
    discount = 1;
    manuDate = QDate::currentDate();
    validDate = QDate::currentDate();
}
Electronics::Electronics(int _owner, QString _name, int _amount, double _price, QDate _manuDate, QDate _validDate) :
    Goods(ETRO,_owner, _name,_amount,_price)
{
    discount = 0.8;
    manuDate = _manuDate;
    validDate = _validDate;
}

double Electronics::getPrice()
{
    QDate currentDate = QDate::currentDate();
    if (currentDate > validDate) {
        return -1;
    } else {
        int days = manuDate.daysTo(currentDate);
        return price - days * 0.2;//price;
    }
}

QString Electronics::getMessage() {
    return "商品名:"+name+" 商品id:"+QString::number(id)+" 卖家:"+QString::number(owner)+" 库存:"+QString::number(amount)+QString(" 价格: %1").arg(price)\
            +" 生产日期:"+manuDate.toString("yyyy/MM/dd")+" 有效期:"+validDate.toString("yyyy/MM/dd");
}

QDataStream &operator<<(QDataStream &out, const Electronics &elet)
{
    QString type = "Electronics";
    out << type << elet.id << elet.owner << elet.name << elet.amount << elet.price <<
           elet.manuDate.toString("yyyy/MM/dd") << elet.validDate.toString("yyyy/MM/dd");
    return out;
}

QDataStream &operator>>(QDataStream &in, Electronics &elet)
{
    QString mDate,vDate;
    in >> elet.id >> elet.owner >> elet.name >> elet.amount >> elet.price >> mDate >> vDate;
    elet.manuDate.fromString(mDate,"yyyy/MM/dd");
    elet.validDate.fromString(vDate,"yyyy/MM/dd");

    return in;
}
