#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QException>
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->login_page);
    ui->login_buyer_rBtn->setChecked(true);
    ui->login_nameEdit->setFocus();
    standardItemModel = new QStandardItemModel(this);
    setWindowTitle("商城客户端");

    udpSocket = new QUdpSocket(this);
    host = "127.0.0.1"; //服务器ip地址,会变动
    port = 8008;
    udpSocket->bind(QHostAddress::Any,8007);

    islogin = UNLOG;
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_login_loginBtn_clicked(){
    bool flag;
    int userType, messageType = LOGIN;
    QString inputName = ui->login_nameEdit->text();
    QString inputPassword = ui->login_pwdEdit->text();

    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    if (ui->login_buyer_rBtn->isChecked())
        userType = BUYER;
    else
        userType = SELLER;
    out << userType << messageType;
    out << inputName << inputPassword;

    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    QByteArray data;
    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> flag;

    if (!flag)
    {
        islogin = UNLOG;
        ui->login_nameEdit->clear();
        ui->login_pwdEdit->clear();
        ui->login_nameEdit->setFocus();
        QMessageBox::warning(this,tr("警告"),tr("登录失败"),QMessageBox::Yes);

    }
    else
    {
        if (ui->login_buyer_rBtn->isChecked())
        {
            islogin = BUYER;
            ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
        }
        else
        {
            islogin = SELLER;
            ui->stackedWidget->setCurrentWidget(ui->seller_mainPage);
        }
    }
}


void MainWindow::on_returnBtn_clicked(){
    if (islogin == BUYER)
        ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->seller_mainPage);
}

void MainWindow::on_goodsList_searchBtn_clicked(){
    QString str = ui->goodsList_searchEdit->text();

    if (func == 1)
    {
        int userType = islogin, messageType = SEARCH;
        qDebug() << islogin;
        QByteArray datagram;
        QDataStream out(&datagram,QIODevice::WriteOnly);

        out << userType << messageType;
        out << str;

        udpSocket->writeDatagram(datagram,datagram.size(),host,port);

        for (int i=0;!udpSocket->hasPendingDatagrams();i++){
            QThread::msleep(1);
            if(i>=2000) {
                QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
                break;
            }
        }

        QStringList list;
        QByteArray data;
        QDataStream in(&data,QIODevice::ReadOnly);
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(),data.size());
        in >> list;
        goods_display(list);
        ui->goodsList_list->setModel(standardItemModel);
    }
    else
    {
        int userType = islogin, messageType = MYSEARCH;
        QByteArray datagram;
        QDataStream out(&datagram,QIODevice::WriteOnly);

        out << userType << messageType;
        out << str;

        udpSocket->writeDatagram(datagram,datagram.size(),host,port);

        for (int i=0;!udpSocket->hasPendingDatagrams();i++){
            QThread::msleep(1);
            if(i>=2000) {
                QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
                break;
            }
        }

        QStringList list;
        QByteArray data;
        QDataStream in(&data,QIODevice::ReadOnly);
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(),data.size());
        in >> list;
        goods_display(list);
        ui->goodsList_list->setModel(standardItemModel);
    }
}

void MainWindow::on_getInfoBtn_clicked(){
    int userType = islogin,messageType = INFO;
    QString info;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    QByteArray data;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> info;

    ui->info_text->setText(info);
    ui->stackedWidget->setCurrentWidget(ui->info_page);
}

void MainWindow::goods_display(QStringList strList){
    standardItemModel->clear();
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        string = "  " + string + "   ";
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::gray);
            linearGrad.setColorAt(1, Qt::gray);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModel->appendRow(item);
    }
}

void MainWindow::on_logoutBtn_clicked(){
    bool flag;
    int userType = islogin, messageType = LOGOUT;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;

    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;

    if (flag)
    {
        islogin = UNLOG;
        ui->stackedWidget->setCurrentWidget(ui->login_page);
        ui->login_nameEdit->clear();//清空用户名输入框
        ui->login_pwdEdit->clear();//清空密码输入框
        ui->login_nameEdit->setFocus();//将光标转到用户名输入框
    }
}

void MainWindow::on_goodsListBtn_clicked(){
    func = 1;

    int userType = BUYER,messageType = GOODSLIST;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;

    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    QStringList goodsList;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> goodsList;

    goods_display(goodsList);
    ui->goodsList_list->setModel(standardItemModel);
    ui->goodsList_searchEdit->clear();
    ui->stackedWidget->setCurrentWidget(ui->goodsList_page);
    ui->goodsList_addBtn->show();
    ui->goodsList_changeBtn->hide();
    ui->goodsList_idEdit->setValue(0);
    ui->goodsList_amountEdit->setValue(0);
    ui->goodsList_amountEdit->setMaximum(100);
    ui->goodsList_idEdit->setMaximum(50);
}

void MainWindow::on_cartBtn_clicked()
{
    int userType = BUYER,messageType = GETCART;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;

    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    QStringList cartList;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> cartList;
    goods_display(cartList);
    ui->cartList_list->setModel(standardItemModel);
    ui->stackedWidget->setCurrentWidget(ui->cart_page);
}

void MainWindow::on_returnBtn_2_clicked(){
    if (islogin == BUYER)
        ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->seller_mainPage);
}

void MainWindow::on_returnBtn_3_clicked(){
    ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
}
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
}
void MainWindow::on_returnBtn_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
}

void MainWindow::on_chargeBtn_clicked(){
    ui->stackedWidget->setCurrentWidget(ui->charge_page);
    ui->charge_numEdit->setMaximum(10000);
    ui->charge_numEdit->setValue(0);
    int userType = BUYER,messageType = CARDAMOUNT;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    int n = 0,num;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> n;
    for (int i = 0; i <n; i++){
        in >> num;
        ui->charge_comboBox->addItem(QString::number(num));
    }
}



void MainWindow::on_logoutBtn_2_clicked(){
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    ui->login_nameEdit->clear();//清空用户名输入框
    ui->login_pwdEdit->clear();//清空密码输入框
    ui->login_nameEdit->setFocus();//将光标转到用户名输入框

    islogin = UNLOG;
}

void MainWindow::on_getInfoBtn_2_clicked(){
    int userType = islogin,messageType = INFO;
    QString info;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    QByteArray data;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> info;

    ui->info_text->setText(info);
    ui->stackedWidget->setCurrentWidget(ui->info_page);

}

void MainWindow::on_goodsListBtn_2_clicked(){
    func = 1;

    int userType = SELLER,messageType = GOODSLIST;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    QStringList goodsList;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> goodsList;

    ui->goodsList_searchEdit->clear();
    ui->stackedWidget->setCurrentWidget(ui->goodsList_page);
    ui->goodsList_addBtn->hide();
    ui->goodsList_changeBtn->show();
    goods_display(goodsList);
    ui->goodsList_list->setModel(standardItemModel);
}

void MainWindow::on_myGoodsListBtn_clicked(){
    func = 2;

    int userType = SELLER,messageType = MYGOODSLIST;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    QStringList goodsList;
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> goodsList;

    ui->goodsList_searchEdit->clear();
    goods_display(goodsList);
    ui->goodsList_list->setModel(standardItemModel);
    ui->stackedWidget->setCurrentWidget(ui->goodsList_page);
}

void MainWindow::on_login_signinBtn_clicked(){
    int userType,messageType = SIGNIN;
    bool flag;
    QString name = ui->login_nameEdit->text();
    QString password = ui->login_pwdEdit->text();

    if (ui->login_buyer_rBtn->isChecked())
        userType = BUYER;
    else
        userType = SELLER;

    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    out << name << password;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }  //等待有消息发过来
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;

    if (flag)
        QMessageBox::warning(this,tr("提示"),tr("注册成功!"),QMessageBox::Yes);
    else
        QMessageBox::warning(this,tr("警告"),tr("用户已存在，无法注册!"),QMessageBox::Yes);

    ui->login_nameEdit->clear();//清空用户名输入框
    ui->login_pwdEdit->clear();//清空密码输入框
    ui->login_nameEdit->setFocus();//将光标转到用户名输入框
}

void MainWindow::on_purchaseBtn_clicked()
{
    int userType = BUYER, messageType = PURCHASE;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    bool flag;
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;
    if (!flag)
    {
        QMessageBox::warning(this,tr("提示"),tr("购买失败!"),QMessageBox::Yes);
        on_cartBtn_clicked();
    }else{
        QMessageBox::warning(this,tr("提示"),tr("购买成功!"),QMessageBox::Yes);
        on_cartBtn_clicked();
    }
}

void MainWindow::on_goodsList_addBtn_clicked()
{
    if (islogin == SELLER)
    {
        QMessageBox::warning(this,tr("警告"),tr("卖家不能购买商品!"),QMessageBox::Yes);
        return;
    }
    if (ui->goodsList_amountEdit->value() == 0)
    {
        QMessageBox::warning(this,tr("警告"),tr("商品数目必须大于0!"),QMessageBox::Yes);
        return;
    }
    int id = ui->goodsList_idEdit->value();
    int amount = ui->goodsList_amountEdit->value();

    int userType = BUYER, messageType = ADDCART;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;
    out << id << amount;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    bool flag;
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;
    if (flag)
    {
        QMessageBox::warning(this,tr("提示"),tr("加入成功!"),QMessageBox::Yes);
        on_goodsListBtn_clicked();
    }else{
        QMessageBox::warning(this,tr("提示"),tr("哎呀出问题啦，请检查一下提交信息是不是有错误!"),QMessageBox::Yes);
        on_goodsListBtn_clicked();
    }
}

void MainWindow::on_cartList_reAddBtn_clicked()
{
    int id = ui->cartList_idEdit->value();
    int amount = ui->cartList_amountEdit->value();

    int userType = BUYER, messageType = ADDCART;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;
    out << id << amount;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    bool flag;
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;
    if (flag)
    {
        QMessageBox::warning(this,tr("提示"),tr("修改成功!"),QMessageBox::Yes);
        on_cartBtn_clicked();
    }else{
        QMessageBox::warning(this,tr("提示"),tr("哎呀出问题啦，请检查一下提交信息是不是有错误!"),QMessageBox::Yes);
        on_cartBtn_clicked();
    }
}

void MainWindow::on_charge_chargeBtn_clicked(){
    if (abs(ui->charge_numEdit->value()) < 1e-5)
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入大于0的数!"),QMessageBox::Yes);
        return;
    }

    quint8 flag;
    int userType = BUYER, messageType = CHARGE;
    double chargeNum = ui->charge_numEdit->value();
    QString cardPwd = ui->charge_pwdEdit->text();
    int n = ui->charge_comboBox->currentText().toInt();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType;
    out << chargeNum << cardPwd << n;

    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;
    if (flag==3)
    {
        QMessageBox::warning(this,tr("提醒"),tr("对不起，您未绑定银行卡或您的卡已被注销"),QMessageBox::Yes);
        ui->stackedWidget->setCurrentWidget(ui->bind_page);
    }else if (flag==2){
        QMessageBox::warning(this,tr("提醒"),tr("对不起，密码错误"),QMessageBox::Yes);
        ui->charge_numEdit->setValue(0);
    }
    else if (flag==1){
        QMessageBox::warning(this,tr("提醒"),tr("卡上余额不足"),QMessageBox::Yes);
        ui->charge_numEdit->setValue(0);
    }
    else if (flag==0){
        QMessageBox::warning(this,tr("提醒"),tr("充值成功"),QMessageBox::Yes);
        ui->charge_numEdit->setValue(0);
    }
}

void MainWindow::on_returnBtn_4_clicked(){
    return ui->stackedWidget->setCurrentWidget(ui->seller_mainPage);
}

void MainWindow::on_stockBtn_clicked(){
    ui->stoke_amountEdit->setMaximum(1000);
    ui->stoke_priceEdit->setMaximum(10000);
    ui->stoke_amountEdit->clear();
    ui->stoke_nameEdit->clear();
    ui->stoke_priceEdit->clear();
    ui->food_rBtn->setChecked(true);
    ui->mDateEdit->setDate(QDate::currentDate());
    ui->vDateEdit->setDate(QDate::currentDate());
    ui->disDateEdit->setDate(QDate::currentDate());
    ui->stackedWidget->setCurrentWidget(ui->stoke_page);
}

void MainWindow::on_food_rBtn_clicked(){
    ui->label_13->setVisible(true);
    ui->disDateEdit->setVisible(true);
}

void MainWindow::on_elet_rBtn_clicked(){
    ui->label_13->setVisible(false);
    ui->disDateEdit->setVisible(false);
}

void MainWindow::on_living_rBtn_clicked(){
    ui->label_13->setVisible(false);
    ui->disDateEdit->setVisible(false);
}

void MainWindow::on_stoke_stokeBtn_clicked(){
    QString name = ui->stoke_nameEdit->text();
    int amount = ui->stoke_amountEdit->value();
    double price = ui->stoke_priceEdit->value();
    QDate manuDate = ui->mDateEdit->date();
    QDate validDate = ui->vDateEdit->date();
    QDate discDate = ui->disDateEdit->date();

    if (amount == 0)
    {
        QMessageBox::warning(this,tr("警告"),tr("商品数目必须大于0!"),QMessageBox::Yes);
        return;
    }
    if (price == 0)
    {
        QMessageBox::warning(this,tr("警告"),tr("商品价格必须大于0!"),QMessageBox::Yes);
        return;
    }
    if (name == "")
    {
        QMessageBox::warning(this,tr("警告"),tr("商品名不能为空!"),QMessageBox::Yes);
        return;
    }

    int userType = SELLER, messageType = STOCK, goodsType;
    if (ui->food_rBtn->isChecked())
        goodsType = FOOD;
    else if (ui->elet_rBtn->isChecked())
        goodsType = ETRO;
    else
        goodsType = LIVE;

    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << userType << messageType << goodsType;
    out << name << price << amount;
    if (goodsType == FOOD)
        out << manuDate << validDate << discDate;
    else
        out << manuDate << validDate;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }


    bool flag;
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;

    if (flag)
    {
        ui->stoke_nameEdit->clear();
        ui->stoke_amountEdit->clear();
        ui->stoke_priceEdit->clear();
        ui->mDateEdit->setDate(QDate::currentDate());
        ui->vDateEdit->setDate(QDate::currentDate());
        ui->disDateEdit->setDate(QDate::currentDate());
        QMessageBox::warning(this,tr("提示"),tr("进货成功!"),QMessageBox::Yes);
    }
}

void MainWindow::on_mDateEdit_dateChanged(const QDate &date){
    if (date > QDate::currentDate())
        QMessageBox::warning(this,tr("警告"),tr("不可能的商品!"),QMessageBox::Yes);
}

void MainWindow::on_vDateEdit_dateChanged(const QDate &date){
    if (date < ui->mDateEdit->date())
        QMessageBox::warning(this,tr("警告"),tr("有效期必须大于生产期!"),QMessageBox::Yes);
}

void MainWindow::on_disDateEdit_dateChanged(const QDate &date){
    if (date > ui->vDateEdit->date())
        QMessageBox::warning(this,tr("警告"),tr("降价期必须小于有效期!"),QMessageBox::Yes);
}

void MainWindow::on_bind_bindBtn_clicked()
{
    int userType = BUYER, messageType = BINDCARD;
    int idNum = (ui->bind_numEdit->text()).toInt();
    QString pwd = ui->bind_pwdEdit->text();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType <<messageType << idNum << pwd;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=4000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }
    bool flag;
    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    in >> flag;
    if (flag) {
        QMessageBox::warning(this,tr("警告"),tr("绑定成功"),QMessageBox::Yes);
        ui->stackedWidget->setCurrentWidget(ui->buyer_mainPage);
    }else QMessageBox::warning(this,tr("警告"),tr("该卡已绑定，或信息错误"),QMessageBox::Yes);
}

void MainWindow::on_bindButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bind_page);
    ui->bind_numEdit->setValidator( new QIntValidator(0, 1000, this) );
}

void MainWindow::on_goodsList_changeBtn_clicked()
{
    int userType = SELLER,messageType = GETGOOD;
    int id = ui->goodsList_idEdit->value();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << userType << messageType << id;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);

    for (int i=0;!udpSocket->hasPendingDatagrams();i++){
        QThread::msleep(1);
        if(i>=2000) {
            QMessageBox::warning(this,tr("警告"),tr("通讯失败"),QMessageBox::Yes);
            break;
        }
    }

    QByteArray data;
    QDataStream in(&data,QIODevice::ReadOnly);
    data.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(data.data(),data.size());
    bool flag;
    in >> flag;
    if (flag){
        ui->stackedWidget->setCurrentWidget(ui->stoke_page);
        QString name;
        int amount;
        in >> name >> amount;
        ui->stoke_nameEdit->setText(name);
        ui->stoke_amountEdit->setValue(amount);
    }else QMessageBox::warning(this,tr("警告"),tr("您没有权限或该商品已不存在"),QMessageBox::Yes);
}
