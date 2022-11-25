#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QSqlDatabase>
#include <QListWidgetItem>
#include "messenger.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init_connect();
    void tray_con();
    void on_connect_db();
    void on_start();
    Messenger msgr;
    QAction *quit_action;
    QString fileName;
    bool loaded=false;
    QTcpServer m_server;
    bool on_check_connectlist(QString connected_ip);
    QList <QTcpSocket*> client_list;
    void createUsersList();
private slots:
    void on_actionexit_triggered();
    void on_show_detail(QListWidgetItem*);
    void on_btn_save_clicked();
    void on_btn_remove_clicked();
    void on_pitch_changed(int val);

    void on_serverbtn_clicked();
    void loadStyleSheet(QString sheet_name);
    void on_cmbTheme_currentIndexChanged(const QString &arg1);
    void onconnected();
    void onconnectfromclient();
    void onTimeout();
private:
    Ui::MainWindow *ui;
    QString mediadir;
    QSqlDatabase m_db;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};

#endif // MAINWINDOW_H
