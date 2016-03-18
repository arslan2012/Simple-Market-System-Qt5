#include "livinggoods.h"

LivingGoods::LivingGoods() :
    Goods(LIVE)
{
    manuDate = QDate::currentDate();
    validDate = QDate::currentDate();
}

LivingGoods::LivingGoods(int _owner, QString _name, int _amount, double _price, QDate _manuDate, QDate _validDate) :
    Goods(LIVE,_owner,_name,_amount,_price)
{
    manuDate = _manuDate;
    validDate = _validDate;

}
double LivingGoods::getPrice(){
    if (QDate::currentDate() > validDate)
        return -1;
    else
        return price;
}

QString LivingGoods::getMessage() {
    return "商品名:"+name+" 商品id:"+QString::number(id)+" 卖家:"+QString::number(owner)+" 库存:"+QString::number(amount)+QString(" 价格:%1").arg(price)\
            +" 生产日期:"+manuDate.toString("yyyy/MM/dd")+" 有效期:"+validDate.toString("yyyy/MM/dd");

}

QDataStream &operator<< (QDataStream &out, const LivingGoods &live)
{
    QString type("LivingGoods");
    out << type << live.id << live.owner << live.name << live.amount << live.price <<
           live.manuDate.toString("yyyy/MM/dd") << live.validDate.toString("yyyy/MM/dd");
    return out;
}

QDataStream &operator>> (QDataStream &in, LivingGoods &live)
{
    QString mDate,vDate;
    in >>live.id >> live.owner >> live.name >> live.amount >> live.price >> mDate >> vDate;
    live.manuDate.fromString(mDate,"yyyy/MM/dd");
    live.validDate.fromString(vDate,"yyyy/MM/dd");
    return in;
}
