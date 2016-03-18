#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fout("file.dat"),
    fin("file.dat"){
    ui->setupUi(this);
    setWindowTitle("银行服务端");

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 1337);
    //绑定接收信号槽
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
    this->read_file();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(!fout.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"警告",tr("文件无法打开"),QMessageBox::Yes);
        return;
    }
    QDataStream out(&fout);

    for (size_t i = 0; i < users.size(); i++){
        out << *users[i];
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
        else{
            users.push_back(new user());
            size_t i = users.size() - 1;
            in >> *users[i];
        }
    }
    fin.close();
}
int MainWindow::generateCard(quint8 _bankChoice){
    int num;
            num=_bankChoice*100+(qrand()/100);
        for (size_t i=0; i < users.size(); i++) {
            if (users[i]->getCardNum() == num){
                num++;
                i=0;
            }//end if
        }//end for
    return num;
}

void MainWindow::processPendingDatagrams(){
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        QHostAddress host;
        quint16 Port_tx;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(),&host,&Port_tx);
        QDataStream in(&datagram, QIODevice::ReadOnly);
        quint8 messageType;
        in >> messageType;
            if (messageType == LOGIN){
                ui->textBrowser->append("USER:LOGIN");
                size_t i = 0;
                QString idNum,pwd;
                in >> idNum >> pwd;
                for ( ; i < users.size(); i++){
                    if (users[i]->login(idNum,pwd)){
                        userRcd[host.toString()] = i;
                        break;
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                bool res;
                if (i == users.size())
                    res = false;
                else
                    res = true;
                out << res;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == SIGNUP){
                ui->textBrowser->append("USER:SIGNUP");
                size_t i = 0;
                bool res;
                QString idNum,pwd;
                quint8 cho;
                in >> idNum >> pwd >> cho;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                for (; i < users.size(); i++) {
                    if (users[i]->getId() == idNum)
                        break;
                }
                if (i == users.size()) {
                    res = true;
                    users.push_back(new user(idNum,pwd,cho,this->generateCard(cho)));
                    out << res << users[users.size()-1]->getCardNum();
                }
                else{
                    res = false;
                    out << res;
                }
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == INFO){
                ui->textBrowser->append("USER:INFO");
                int pos = userRcd[host.toString()];
                QString info = users[pos]->getInformation();

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << info;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == LOGOUT){
                ui->textBrowser->append("USER:LOGOUT");
                int pos = userRcd[host.toString()];
                users[pos]->logout();
                userRcd.remove(host.toString());

                bool flag = true;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == CHARGE){
                ui->textBrowser->append("USER:CHARGE");
                double number;
                in >> number;

                int pos = userRcd[host.toString()];
                users[pos]->chargeBalance(number);

                bool flag = true;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);

            }
            else if (messageType == WITHDRAW){
                ui->textBrowser->append("USER:WITHDRAW");
                int pos = userRcd[host.toString()];
                double amount;
                in >> amount;

                bool flag = users[pos]->withdraw(amount);
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == CHANGEPWD){
                ui->textBrowser->append("USER:CHANGEPWD");
                int pos = userRcd[host.toString()];
                QString pwd;
                in >> pwd;
                users[pos]->setPwd(pwd);
                bool flag = true;
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == GETBANK){
                ui->textBrowser->append("USER:GETBANK");
                QFile fbank("bank.txt");
                if(!fbank.open(QIODevice::ReadOnly)) {
                    QMessageBox::warning(this,"警告",tr("文件无法打开"),QMessageBox::Yes);
                    return;
                }
                quint8 sum;
                QString line[32];
                for (sum = 0;!fbank.atEnd();sum++) {
                        line[sum] = fbank.readLine();
                    }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << sum;
                for (quint8 i=0;i<sum;i++) out << line[i];
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
                fbank.close();
            }
            if (messageType == BINDCARD){
                ui->textBrowser->append("MARKET:BINDCARD");
                size_t i = 0;
                int cardNum;
                QString password;
                in >> cardNum >> password;
                for ( ; i < users.size(); i++){
                    if (users[i]->getCardNum()==cardNum){
                        break;
                    }
                }
                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                bool res;
                if (i == users.size())
                    res = false;
                else
                    res = true;
                out << res;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
            else if (messageType == CARDWITHDRAW){
                ui->textBrowser->append("MARKET:CARDWITHDRAW");
                size_t i = 0;
                double amount;
                int cardNum;
                QString password;
                quint8 flag;
                in >> amount >> cardNum >> password;
                for ( ; i < users.size(); i++){
                    if (users[i]->getCardNum()==cardNum){
                        break;
                    }
                }
                if(i==users.size()) flag=3;
                else if(users[i]->login(users[i]->getId(),password)==false) flag=2;
                else if(users[i]->withdraw(amount)==false) flag = 1;
                else flag = 0;

                QByteArray data;
                QDataStream out(&data,QIODevice::WriteOnly);
                out << flag;
                udpSocket->writeDatagram(data,data.size(),host,Port_tx);
            }
    }// end while
} //end function

