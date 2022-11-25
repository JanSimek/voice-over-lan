#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messenger.h"
#include <QHostAddress>
#include <QSound>
#include <QDebug>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QDirIterator>
#include <QTextCodec>
#include <QTimer>

#define server_port 30011

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tray_con();
    init_connect();
    QTimer m_timer;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer.start(1000);
    on_connect_db();
 //   msgr.vso->startListen(server_port);

    QDirIterator it("./Resource/themes/", QStringList() << "*.qss", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());
        ui->cmbTheme->addItem(it.next().toLatin1());
    }


    QFile MyFile("themes.txt");
    if(MyFile.exists()){
        MyFile.open(QIODevice::ReadWrite);
        QTextStream in (&MyFile);
        QString line;
        QStringList list;
         //   QList<QString> nums;
        QStringList nums;
        QRegExp rx("[:]");
        line = in.readLine();
  QString stylesheet;
        if (line.contains(":")) {
            list = line.split(rx);
                qDebug() << "theme" <<  list.at(1).toLatin1();
                stylesheet =  list.at(1).toLatin1();
          loadStyleSheet( list.at(1).toLatin1());

                MyFile.close();
        }

  fileName=stylesheet;
        QFile file(stylesheet);

        file.open(QIODevice::Text | QIODevice::ReadOnly);
        QString content;
        while(!file.atEnd())
            content.append(file.readLine());
    }
  loaded=true;
}

MainWindow::~MainWindow()
{
    m_db.close();
    delete ui;
}

void MainWindow::onTimeout()
{
    for(int i = 0; i < client_list.count(); i++)
    {
        if(!client_list.at(i)->state()){
            QString dis_con_ip = client_list.at(i)->peerAddress().toString();
            dis_con_ip = dis_con_ip.right(dis_con_ip.length() - dis_con_ip.indexOf(":",3)-1);
            int del_num=0;
            for(int j = 0; j < ui->m_connectedip->count(); j++){
                if(ui->m_connectedip->item(j)->text() == dis_con_ip){
                    ui->m_connectedip->removeItemWidget( ui->m_connectedip->item(j));
                    client_list.removeAt(i);
                    return ;
                }
            }
        }
    }
}

void MainWindow::tray_con()
{
    QString mediadir = "./Resource/";
    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "phone.png");
    QIcon oIcon( oPixmap );
    trayIcon = new QSystemTrayIcon(oIcon);
    quit_action = new QAction( "Exit", trayIcon );
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );
    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);
}

void MainWindow::init_connect()
{
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui->m_ips, SIGNAL(itemClicked(QListWidgetItem* )), this, SLOT(on_show_detail(QListWidgetItem* )));
    connect(ui->m_pitch, SIGNAL(valueChanged(int)), this, SLOT(on_pitch_changed(int)));
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(onconnectfromclient()));
}

void MainWindow::on_pitch_changed(int val)
{
    msgr.vso->pitch_val = val;
}

void MainWindow::on_show_detail(QListWidgetItem* ip_item)
{
    QString str = QString("SELECT * FROM user_chat WHERE user_chat.user_ip = '%1';").arg(ip_item->text());
    QSqlQuery query_detail(str);
    while (query_detail.next())
    {
        ui->m_name->setText(query_detail.value(2).toString());
        ui->m_phone->setText(query_detail.value(3).toString());
        ui->m_addr->setText(query_detail.value(4).toString());
        ui->m_extra->setText(query_detail.value(5).toString());
        ui->m_cur_ip->setText(ip_item->text());
    }
    msgr.set_host(ui->m_cur_ip->text().toLatin1());
    QTcpSocket* m_client_soc = new QTcpSocket;
    m_client_soc->connectToHost(ip_item->text(),server_port+1);
    connect(m_client_soc, SIGNAL(connected()), this, SLOT(onconnected()));
    on_start();
}

void MainWindow::onconnectfromclient()
{
    QTcpSocket* client_sock = m_server.nextPendingConnection();
    QString client_ip = client_sock->peerAddress().toString();
    client_ip = client_ip.right(client_ip.length() - client_ip.indexOf(":",3)-1);
    if(on_check_connectlist(client_ip))
    {
        ui->m_connectedip->addItem(client_ip);
        client_list.append(client_sock);
    }
}

void MainWindow::onconnected()
{
    QString con_ip = ui->m_cur_ip->text();
    if(on_check_connectlist(con_ip))
    {
        ui->m_connectedip->addItem(con_ip);
    }
}

bool MainWindow::on_check_connectlist(QString connected_ip)
{
    for(int i = 0; i < ui->m_connectedip->count(); i++)
    {
        if(ui->m_connectedip->item(i)->text() == connected_ip)
        {
            return 0;
        }
    }
    return 1;
}

void MainWindow::on_start()
{
        QSound::play( mediadir.toLatin1() + "sounds/ec2_mono.ogg");
#ifdef FTP
    if (adminftp==0){
    ftpgui = new FTPGUI;
    adminftp=1;
    }
    if (adminftp) { ftpgui->show();}
#endif
}

void MainWindow::on_actionexit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_connect_db()
{
    m_db.close();
    ui->m_ips->clear();
    ui->m_addr->clear();
    ui->m_extra->clear();
    ui->m_phone->clear();
    ui->m_name->clear();
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("./voice_user.db");

    if(m_db.open()) {
        qDebug()<<"Successful database connection";
    }
    else {
        qDebug()<<"Error: failed database connection";
    }

    QSqlQuery query("SELECT * FROM user_chat;");

    while (query.next())
    {
        ui->m_ips->addItem(query.value(1).toString());
    }
}

void MainWindow::on_btn_save_clicked()
{
    QString add_ip = ui->m_cur_ip->text();
    for(int i=0; i<ui->m_ips->count(); i++){
        if( ui->m_ips->item(i)->text() == add_ip){
            int a = QMessageBox::information(this, "InFo", "Already exist!\n Update data of " + add_ip + " ?", QMessageBox::Ok,QMessageBox::Cancel);
            if(a == QMessageBox::Ok){
                QString update_str = QString("UPDATE user_chat SET Name = '%1' , Phone = '%2' , address = '%3' , extra = '%4' WHERE user_ip = '%5';").arg(ui->m_name->text()).arg(ui->m_phone->text()).arg(ui->m_addr->text()).arg(ui->m_extra->toPlainText()).arg(add_ip);
                QSqlQuery record_up(update_str);
                on_connect_db();
                return ;
            } else {
                return;
            }
        }
    }

    int b = QMessageBox::information(this, "InFo", "Insert data of " + add_ip + " ?",  QMessageBox::Ok,QMessageBox::Cancel);
    if (b == QMessageBox::Ok){
        QString insert_str = QString("INSERT INTO user_chat(user_ip) VALUES ( '%1' );").arg(add_ip);
        QSqlQuery insert_query(insert_str);
        on_connect_db();
    }
}

void MainWindow::on_btn_remove_clicked()
{
    QString del_ip = ui->m_ips->currentItem()->text();
    int b = QMessageBox::information(this, "InFo", "Delete data of " + del_ip + " ?",  QMessageBox::Ok,QMessageBox::Cancel);
    if (b == QMessageBox::Ok){
        QString del_str = QString("DELETE FROM user_chat WHERE user_ip = '%1';").arg(del_ip);
        QSqlQuery del_query(del_str);
        on_connect_db();
    }
}

void MainWindow::on_serverbtn_clicked()
{
    msgr.vso->startListen(server_port);
    m_server.listen(QHostAddress::Any, server_port+1);
}

void MainWindow::loadStyleSheet( QString sheet_name)
{
    QFile file(sheet_name);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
}

void MainWindow::on_cmbTheme_currentIndexChanged(const QString &arg1)
{
    if (loaded==true)
    {
    fileName=ui->cmbTheme->currentText();
    QFile file(fileName);

    file.open(QIODevice::Text | QIODevice::ReadOnly);
    QString content;
    while(!file.atEnd())
        content.append(file.readLine());
    file.close();

    loadStyleSheet(ui->cmbTheme->currentText());

    QFile file2("themes.txt");
        if(file2.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
        {
                QTextStream stream(&file2);
                file2.seek(0);
               stream << "theme:" << ui->cmbTheme->currentText().toLatin1()<< endl;
                for (int i = 0; i < ui->cmbTheme->count(); i++)
                {
                 stream << "theme:" << ui->cmbTheme->itemText(i) << endl;
                }
            //                file.write("\n");
               file2.close();
        }

    if (ui->cmbTheme->currentText().toLatin1() != ""){
      //   ui->cmbTheme->currentText().toLatin1();
    }
}

}
