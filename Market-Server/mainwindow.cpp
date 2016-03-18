#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fout("file.dat"),
    fin("file.dat"){
    ui->setupUi(this);
    setWindowTitle("商城服务端");

    udpClientSocket = new QUdpSocket(this);
    udpClientSocket->bind(QHostAddress::Any, 8008);
    //绑定接收信号槽
    connect(udpClientSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

    udpBankSocket = new QUdpSocket(this);
    bankhost = "127.0.0.1";//服务器ip地址,会变动
    port = 1337;
    udpBankSocket->bind(QHostAddress::Any,1336);
    this->read_file();
    this->read_disfile();
}

MainWindow::~MainWindow(){
    delete ui;
    this->write_file();
}
void MainWindow::write_file(){
    if(!fout.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"警告",tr("文件无法打开"),QMessageBox::Yes);
        return;
    }
    QDataStream out(&fout);

    for (size_t i = 0; i < buyers.size(); i++){
        out << (*buyers[i]);
    }
    for (size_t i = 0; i < sellers.size(); i++){
        out << (*sellers[i]);
    }

    for (size_t i = 0; i < goods.size(); i++){
        if (goods[i]->type == FOOD){
            Food *f = (Food *)goods[i];
            out << *f;
        }
        else if (goods[i]->type == ETRO){
            Electronics *elet = (Electronics *)goods[i];
            out << *elet;
        }
        else{
            LivingGoods *live = (LivingGoods *)goods[i];
            out << *live;
        }

    }
    QString str = "EOF";
    out << str;
    fout.close();
}

void MainWindow::read_file(){
    if(!fin.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,"警告",tr("文件无法打开"),QMessageBox::Yes);
        return;
    }
    QDataStream in(&fin);

    while (true){
        QString type;
        in >> type;

        if (type == "EOF")
            break;

        if (type == "Buyer"){
            buyers.push_back(new Buyer());
            size_t i = buyers.size() - 1;
            in >> *buyers[i];
        }
        else if (type == "Seller"){
            sellers.push_back(new Seller());
            size_t i = sellers.size() - 1;
            in >> *sellers[i];
        }
        else if (type == "Food"){
            Food *f = new Food();
            in >> *f;
            goods.push_back((Goods *)f);
        }
        else if (type == "Electronics"){
            Electronics *elet = new Electronics();
            in >> *elet;
            goods.push_back((Goods *)elet);
        }
        else if (type == "LivingGoods"){
            LivingGoods *live = new LivingGoods();
            in >> *live;
            goods.push_back((Goods *)live);
        }
        else{
            QMessageBox::warning(this,"警告",tr("文件读入错误"),QMessageBox::Yes);
            fin.close();
            return;
        }
    }
    fin.close();
}
void MainWindow::read_disfile(){
    QFile fdis("discount.txt");
    if(!fdis.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,"警告",tr("折扣信息文件无法打开,默认不打折"),QMessageBox::Yes);
        eledis = 1;
        fooddis = 1;
        livedis = 1;
        disamount = 1;
        amountdis = 1;
        return;
    }else{
        eledis = fdis.readLine().toInt();
        fooddis = fdis.readLine().toInt();
        livedis = fdis.readLine().toInt();
        disamount = fdis.readLine().toInt();
        amountdis = fdis.readLine().toInt();
    }
    fdis.close();
}

void MainWindow::del_item() {
    for (std::vector<Goods *>::iterator it = goods.begin(); it != goods.end();) {
        if ((*it)->getAmount() == 0)
            it = goods.erase(it);
        else
            it++;
    }
}


void MainWindow::processPendingDatagrams(){
    while(udpClientSocket->hasPendingDatagrams()){
        QByteArray datagram;
        QHostAddress host;
        quint16 Port_tx;
        datagram.resize(udpClientSocket->pendingDatagramSize());
        udpClientSocket->readDatagram(datagram.data(), datagram.size(),&host,&Port_tx);
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int userType,messageType;
        in >> userType >> messageType;

        if (userType == BUYER){
            if (messageType == LOGIN){
                ui->textBrowser->append("BUYER:LOGIN");
                size_t i = 0;
                QString name,pwd;
                in >> name >> pwd;
                for ( ; i < buyers.size(); i++){
                    if (buyers[i]->login(name,pwd)){
                        BuyerRcd[host.toString()] = i;
                        break;
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                bool res;
                if (i == buyers.size())
                    res = false;
                else
                    res = true;
                out << res;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == SIGNIN){
                ui->textBrowser->append("BUYER:SIGNIN");
                size_t i = 0;
                bool res;
                QString name,pwd;
                in >> name >> pwd;
                for (; i < buyers.size(); i++) {
                    if (buyers[i]->getName() == name)
                        break;
                }
                if (i == buyers.size()) {
                    res = true;
                    buyers.push_back(new Buyer(name,pwd,0));
                }
                else
                    res = false;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << res;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == INFO){
                ui->textBrowser->append("BUYER:INFO");
                int pos = BuyerRcd[host.toString()];
                QString info = buyers[pos]->getInformation();

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << info;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == GOODSLIST){
                ui->textBrowser->append("BUYER:GOODSLIST");
                int pos = BuyerRcd[host.toString()];
                QStringList strList = buyers[pos]->getGoodsList(goods);

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << strList;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == LOGOUT){
                ui->textBrowser->append("BUYER:LOGOUT");
                int pos = BuyerRcd[host.toString()];
                buyers[pos]->logout();
                BuyerRcd.remove(host.toString());

                bool flag = true;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == CARDAMOUNT){
                ui->textBrowser->append("BUYER:CARDAMOUNT");
                int pos = BuyerRcd[host.toString()];
                int n = buyers[pos]->cardNum.size();
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << n;
                for (size_t i = 0; i <buyers[pos]->cardNum.size(); i++){
                    out << buyers[pos]->cardNum[i];
                }
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == CHARGE){
                ui->textBrowser->append("BUYER:CHARGE");
                double number;
                QString cardPwd;
                int n;
                quint8 flag;
                in >> number >> cardPwd >> n;
                int pos = BuyerRcd[host.toString()];
                if (n>99){
                    quint8 messageType = CARDWITHDRAW;
                    QByteArray datagramBank;
                    QDataStream outBank(&datagramBank,QIODevice::WriteOnly);
                    outBank << messageType << number << n << cardPwd;
                    udpBankSocket->writeDatagram(datagramBank,datagramBank.size(),bankhost,port);
                    for (int i=0;!udpBankSocket->hasPendingDatagrams();i++){
                        QThread::msleep(1);
                        if(i>=2000) {
                            QMessageBox::warning(this,tr("警告"),tr("银行通讯失败"),QMessageBox::Yes);
                            break;
                        }
                    }
                    QByteArray dataBank;
                    QDataStream inBank(&dataBank,QIODevice::ReadOnly);
                    dataBank.resize(udpBankSocket->pendingDatagramSize());
                    udpBankSocket->readDatagram(dataBank.data(),dataBank.size());
                    inBank >> flag;
                    if (flag == 0) buyers[pos]->chargeBalance(number);
                }else flag = 3;
                if (flag == 3) {
                    for (size_t i = 0; i <buyers[pos]->cardNum.size(); i++){
                        if (buyers[pos]->cardNum[i] == n){
                            buyers[pos]->cardNum.erase(buyers[pos]->cardNum.begin()+i);
                            break;
                        }
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);

            }
            else if (messageType == SEARCH){
                ui->textBrowser->append("BUYER:SEARCH");
                qDebug() << "str" ;
                QString str;
                QStringList list;
                in >> str;

                int pos = BuyerRcd[host.toString()];
                list = buyers[pos]->getGoodsList(goods,str);
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << list;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == PURCHASE){
                ui->textBrowser->append("BUYER:PURCHASE");
                int pos = BuyerRcd[host.toString()];
                size_t j = 0,i = 0;
                double sum = 0;
                bool flag = true;
                for (; j <buyers[pos]->cartId.size(); j++)
                    for (; i <goods.size(); i++){
                        if (goods[i]->getId() == buyers[pos]->cartId[j]){
                            sum+=goods[i]->getPrice();
                        }
                    }
                if (sum > buyers[pos]->getBalance()) flag = false;
                else{
                    for (j = 0; j < buyers[pos]->cartId.size(); j++)
                        for (i = 0; i < goods.size(); i++){
                            if (goods[i]->getId() == buyers[pos]->cartId[j]){
                                if (purchase(pos,buyers[pos]->cartId[j],buyers[pos]->cartAmount[j])){
                                    buyers[pos]->cartId.erase(buyers[pos]->cartId.begin()+j);
                                    buyers[pos]->cartAmount.erase(buyers[pos]->cartAmount.begin()+j);
                                }else{
                                    flag = false;
                                    break;
                                }
                            }
                        }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == GETCART){
                ui->textBrowser->append("BUYER:GETCART");
                int pos = BuyerRcd[host.toString()];
                size_t j = 0,i = 0;
                QStringList list;
                for (; j <buyers[pos]->cartId.size(); j++)
                    for (; i <goods.size(); i++){
                        if (goods[i]->getId() == buyers[pos]->cartId[j])
                            list.append("商品名:"+goods[i]->getName()+" 商品id:"+QString::number(goods[i]->getId())+" 购物车内件数:"+QString::number(buyers[pos]->cartAmount[j])+" 卖家:"+QString::number(goods[i]->getOwner())+" 库存:"+QString::number(goods[i]->getAmount())+QString(" 单价: %1").arg(goods[i]->getPrice()));
                    }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << list;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == ADDCART){
                ui->textBrowser->append("BUYER:ADDCART");
                int pos = BuyerRcd[host.toString()];
                int id,amount;
                bool flag = false;
                in >> id >> amount;
                size_t i = 0;
                for (; i <goods.size(); i++){
                    if (goods[i]->getId() == id)
                        break;
                }
                if (i == goods.size())
                    flag = false;
                else if (goods[i]->getAmount() < amount)
                    flag = false;
                else{
                    flag = true;
                    for (; i <buyers[pos]->cartId.size(); i++){
                        if (buyers[pos]->cartId[i] == id){
                            if (amount==0){
                                buyers[pos]->cartId.erase(buyers[pos]->cartId.begin()+i);
                                buyers[pos]->cartAmount.erase(buyers[pos]->cartAmount.begin()+i);
                            }
                            else buyers[pos]->cartAmount[i] = amount;
                            break;
                        }
                    }
                    if (i == buyers[pos]->cartId.size()){
                        buyers[pos]->cartId.push_back(id);
                        buyers[pos]->cartAmount.push_back(amount);
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == BINDCARD){
                ui->textBrowser->append("BUYER:BINDCARD");
                int idNum;
                size_t i = 0;
                QString pwd;
                bool flag;
                in >> idNum >> pwd;
                int pos = BuyerRcd[host.toString()];
                for (;i < buyers[pos]->cardNum.size();i++){
                    if (buyers[pos]->cardNum[i] == idNum){
                        break;
                    }
                }
                if (i == buyers[pos]->cardNum.size()){
                quint8 BankMes=TESTCARD;
                QByteArray dataBank;
                QDataStream outBank(&dataBank,QIODevice::WriteOnly);
                outBank << BankMes <<idNum << pwd;
                udpBankSocket->writeDatagram(dataBank,dataBank.length(),bankhost,port);
                for (int i=0;!udpBankSocket->hasPendingDatagrams();i++){
                    QThread::msleep(1);
                    if(i>=2000) {
                        QMessageBox::warning(this,tr("警告"),tr("银行通讯失败"),QMessageBox::Yes);
                        break;
                    }
                }
                QByteArray dataIn;
                QDataStream in(&dataIn,QIODevice::ReadOnly);
                dataIn.resize(udpBankSocket->pendingDatagramSize());
                udpBankSocket->readDatagram(dataIn.data(),dataIn.size());
                in >> flag;
                }else flag = false;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                if (flag) buyers[pos]->cardNum.push_back(idNum);
                udpClientSocket->writeDatagram(data,data.length(),host,Port_tx);
            }
        }// end userType
        else{
            if (messageType == LOGIN){
                ui->textBrowser->append("SELLER:LOGIN");
                size_t i = 0;
                QString name,pwd;
                in >> name >> pwd;
                for ( ; i < sellers.size(); i++){
                    if (sellers[i]->login(name,pwd)){
                        SellerRcd[host.toString()] = i;
                        break;
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                bool res;
                if (i == sellers.size())
                    res = false;
                else
                    res = true;
                out << res;
                udpClientSocket->writeDatagram(data,data.length(),host,Port_tx);
            }
            else if (messageType == SIGNIN){
                ui->textBrowser->append("SELLER:SIGNIN");
                size_t i = 0;
                bool res;
                QString name,pwd;
                in >> name >> pwd;
                for (; i < sellers.size(); i++) {
                    if (sellers[i]->getName() == name)
                        break;
                }
                if (i == sellers.size()) {
                    res = true;
                    sellers.push_back(new Seller(name,pwd,0));
                }
                else
                    res = false;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << res;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == INFO){
                ui->textBrowser->append("SELLER:INFO");
                int pos = SellerRcd[host.toString()];
                QString info = sellers[pos]->getInformation();

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << info;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == GOODSLIST){
                ui->textBrowser->append("SELLER:GOODSLIST");
                int pos = SellerRcd[host.toString()];
                QStringList strList = sellers[pos]->getGoodsList(goods);

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << strList;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == GETGOOD){
                ui->textBrowser->append("SELLER:GETGOOD");
                int id;
                in >> id;
                bool flag = true;
                size_t i = 0;
                int pos = SellerRcd[host.toString()];
                for (; i < goods.size(); i++) {
                    if (goods[i]->getId() == id)
                        break;
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                if (i == goods.size()) {flag = false; out << flag;}
                else if (goods[i]->getOwner() != sellers[pos]->getID()) {flag = false; out << flag;}
                else {flag = true; out << flag << goods[i]->getName() << goods[i]->getAmount();}
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
                        goods.erase(goods.begin()+i);
            }
            else if (messageType == SEARCH){
                ui->textBrowser->append("SELLER:SEARCH");
                QString str;
                QStringList list;
                in >> str;

                int pos = SellerRcd[host.toString()];
                list = sellers[pos]->getGoodsList(goods,str);
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << list;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == MYGOODSLIST){
                ui->textBrowser->append("SELLER:MYGOODSLIST");
                QStringList list;

                int pos = SellerRcd[host.toString()];
                list = sellers[pos]->getMyGoodsList(goods);
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << list;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == MYSEARCH){
                ui->textBrowser->append("SELLER:MYSEARCH");
                QString str;
                QStringList list;
                in >> str;

                int pos = SellerRcd[host.toString()];
                list = sellers[pos]->getMyGoodsList(goods,str);
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << list;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == STOCK){
                ui->textBrowser->append("SELLER:STOCK");
                int pos = SellerRcd[host.toString()];
                int owner = sellers[pos]->getID();

                int goodsType;
                QString name;
                double price;
                int amount;
                in >> goodsType;
                in >> name >> price >> amount;
                for (size_t i = 0; i < goods.size(); i++) {
                    if (goods[i]->getName() == name)
                        goods.erase(goods.begin()+i);
                }
                if (goodsType == FOOD){
                    QDate manuDate;
                    QDate validDate;
                    QDate discDate;

                    in >> manuDate >> validDate >> discDate;
                    goods.push_back((Goods *)new Food(owner,name,amount,price,0.9,manuDate,discDate,validDate));
                }
                else if (goodsType == ETRO){
                    QDate manuDate;
                    QDate validDate;

                    in >> manuDate >> validDate;
                    goods.push_back((Goods *)new Electronics(owner,name,amount,price,manuDate,validDate));
                }
                else{
                    QDate manuDate;
                    QDate validDate;

                    in >> manuDate >> validDate;
                    goods.push_back((Goods *)new LivingGoods(owner,name,amount,price,manuDate,validDate));
                }
                bool flag = true;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpClientSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
        }// end userType
    }// end while
} //end function


bool MainWindow::purchase(int pos, int id, int amount){
    size_t i = 0;
    double money;
    for (; i <goods.size(); i++){
        if (goods[i]->getId() == id)
            break;
    }
    if (i == goods.size())
        return false;
    if (goods[i]->getAmount() < amount)
        return false;
    if (goods[i]->getType()=="Electronics")
        money = goods[i]->getPrice() * amount * eledis;
    else if (goods[i]->getType()=="Food")
        money = goods[i]->getPrice() * amount * fooddis;
    else money = goods[i]->getPrice() * amount * livedis;
    if (amount >= disamount) money *= amountdis;
    int status = buyers[pos]->purchase(money);
    if (status == 0) { //成功买入,买家钱减少，卖家钱变多，商品数目减少，同时更新列表
        for (size_t i = 0; i < sellers.size(); i++) {
            if (sellers[i]->getID() == goods[i]->getOwner())
                sellers[i]->earning(money);
            break;
        }
        goods[i]->decAmount(amount);
        del_item();
        return true;
    }
    else
        return false;
}
