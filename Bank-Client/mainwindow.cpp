#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("银行客户端");
    udpSocket = new QUdpSocket(this);
    host = "127.0.0.1"; //服务器ip地址,会变动
    port = 1337;
    udpSocket->bind(QHostAddress::Any,1336);
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_loginBtn_clicked()
{
    bool flag;
    quint8 messageType = LOGIN;
    QString inputName = ui->login_nameEdit->text();
    QString inputPassword = ui->login_pwdEdit->text();

    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << messageType << inputName << inputPassword;

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
        islogin = false;
        ui->login_nameEdit->clear();
        ui->login_pwdEdit->clear();
        ui->login_nameEdit->setFocus();
        QMessageBox::warning(this,tr("警告"),tr("登录失败"),QMessageBox::Yes);

    }
    else
    {
            islogin = true;
            ui->stackedWidget->setCurrentWidget(ui->mainPage);
    }
}

void MainWindow::on_login_signinBtn_clicked()
{
    QString name = ui->login_nameEdit->text();
    QString password = ui->login_pwdEdit->text();
    ///////////////////先从服务器下载银行信息/////////
    quint8 messageType = GETBANKS;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << messageType;
    udpSocket->writeDatagram(datagram,datagram.size(),host,port);
    quint8 sum;
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
    in >> sum;
    /////////用获得的银行信息创建有选择的注册界面/////////
    ui->stackedWidget->setCurrentWidget(ui->signup_page);
    ui->signup_nameEdit->setText(name);
    ui->signup_pwdEdit->setText(password);
    for (quint8 i=0;i<sum;i++){
        QString a;
        in >> a;
        ui->comboBox->addItem(a);
    }
}

void MainWindow::on_signup_signinBtn_clicked()
{
    quint8 messageType = SIGNUP;
    bool flag;
    QString name = ui->signup_nameEdit->text();
    QString password = ui->signup_pwdEdit->text();
    quint8 cho = ui->comboBox->currentIndex()+1;

    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << messageType << name << password << cho;
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

    if (flag){
        int cardnum;
        in >> cardnum;
        QString text = QString("注册成功!卡号为：")+QString::number(cardnum);
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText(text);
        msgBox->show();

        //QMessageBox::warning(this,tr("提示"),tr(text),QMessageBox::Yes);
    }
    else
        QMessageBox::warning(this,tr("警告"),tr("用户已存在，无法注册!"),QMessageBox::Yes);
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    ui->login_nameEdit->clear();//清空用户名输入框
    ui->login_pwdEdit->clear();//清空密码输入框
    ui->login_nameEdit->setFocus();//将光标转到用户名输入框
}

void MainWindow::on_chargeBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->charge_page);
    ui->charge_numEdit->setMaximum(10000);
    ui->charge_numEdit->setValue(0);
}

void MainWindow::on_getInfoBtn_clicked()
{
    quint8 messageType = INFO;
    QString info;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out << messageType;
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

void MainWindow::on_logoutBtn_clicked()
{
    bool flag;
    quint8 messageType = LOGOUT;
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << messageType;

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
        islogin = false;
        ui->stackedWidget->setCurrentWidget(ui->login_page);
        ui->login_nameEdit->clear();//清空用户名输入框
        ui->login_pwdEdit->clear();//清空密码输入框
        ui->login_nameEdit->setFocus();//将光标转到用户名输入框
    }
}

void MainWindow::on_withdrawBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->withdraw_page);
    ui->withdraw_numEdit->setMaximum(10000);
    ui->withdraw_numEdit->setValue(0);
}

void MainWindow::on_returnBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::on_returnBtn_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::on_returnBtn_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::on_charge_chargeBtn_clicked()
{
    if (abs(ui->charge_numEdit->value()) < 1e-5)
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入大于0的数!"),QMessageBox::Yes);
        return;
    }

    bool flag;
    quint8 messageType = CHARGE;
    double chargeNum = ui->charge_numEdit->value();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << messageType << chargeNum;

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
        QMessageBox::warning(this,tr("提醒"),tr("充值成功"),QMessageBox::Yes);
        ui->charge_numEdit->setValue(0);
    }
}

void MainWindow::on_withdraw_withdrawBtn_clicked()
{
    if (abs(ui->withdraw_numEdit->value()) < 1e-5)
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入大于0的数!"),QMessageBox::Yes);
        return;
    }

    bool flag;
    quint8 messageType = WITHDRAW;
    double withNum = ui->withdraw_numEdit->value();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << messageType << withNum;

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
        QMessageBox::warning(this,tr("提醒"),tr("取款成功"),QMessageBox::Yes);
        ui->withdraw_numEdit->setValue(0);
    }else{
        QMessageBox::warning(this,tr("提醒"),tr("余额不足"),QMessageBox::Yes);
        ui->withdraw_numEdit->setValue(0);
    }
}

void MainWindow::on_PwdBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pwd_page);
}

void MainWindow::on_pwd_pwdBtn_clicked()
{
    bool flag;
    quint8 messageType = CHANGEPWD;
    QString pwd = ui->pwd_pwdEdit->text();
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);

    out << messageType << pwd;

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
        QMessageBox::warning(this,tr("提醒"),tr("修改成功"),QMessageBox::Yes);
        ui->charge_numEdit->setValue(0);
    }
}
