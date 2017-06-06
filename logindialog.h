#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>

#include "registerdialog.h"
#include "app/mysqlapi.h"
#include "customerwindget.h"
#include "widget.h"             //������

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void InitForm();

    //���浱ǰ���û�������
    void WriteCurrentUser(QString tablename,QString username,
                          QString usertype,QString userpwd );

    //��������������
    int FindTableIsEmpty(QString tablename);

    //ɾ����������
    void DeleteTableContent(QString tablename);
    
private slots:
    void on_btnLogin_clicked();

    void on_btnMenu_Min_clicked();

    void on_btnMenu_Close_clicked();

    void on_btnresgister_clicked();

    void on_radioButtonCustomer_clicked();

    void on_radioButtonAdmin_clicked();


protected:
    bool eventFilter(QObject *obj, QEvent *event);//�����¼�����Ӧ
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::LoginDialog *ui;

    int total;
    MysqlApi *mysql;
    RegisterDialog *registerdialog;     //ע������
    Widget *mainwidget;                 //ϵͳ������
    CustomerWindget *customewidget;     //�˿ͺ�����½����

    QRect location;
    QPoint mousePoint;
    bool mousePressed;
};

#endif // LOGINDIALOG_H
