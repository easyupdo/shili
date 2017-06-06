#include "logindialog.h"
#include "ui_logindialog.h"
#include "app/iconhelper.h"
#include "app/myapp.h"



LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->InitForm();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::InitForm()
{
    total = 0;
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->txtUserPwd->setFocus();

    customewidget = new CustomerWindget;


    //mysql->BindData("StaffName","Staff",ui->txtUserName);
    location = this->geometry();
    mousePressed = false;
    //????????????,?????????????????
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    ui->labCompany->setText(tr("???????????????--???????????"));
    //ui->radioButtonAdmin->setCheckable(true);

    ui->btnLogin->setToolTip(tr("???"));
    ui->btnresgister->setToolTip(tr("???"));
    ui->radioButtonAdmin->setToolTip(tr("?????"));
    ui->radioButtonCustomer->setToolTip(tr("???"));
}

/*
 *????????????›Ì????????????
 *?????????????????????????????????????????
 *?????????????????§Ø???????????????????????
*/
void LoginDialog::WriteCurrentUser(QString tablename, QString username, QString usertype, QString userpwd)
{
    if(FindTableIsEmpty("currentuser") != 0)
    {
        //deldete table
        this->DeleteTableContent("currentuser");
        qDebug() <<"deltet table ok";
    }
    else
    {
        QSqlQuery query;
        QString sql = "insert into "+tablename+" values(:UserName,:UserType,:UserPwd)";
        qDebug() <<sql;
        query.prepare(sql);
        query.bindValue(":UserName",username);
        query.bindValue(":UserType",usertype);
        query.bindValue(":UserPwd",userpwd);
        query.setForwardOnly(true);
        query.exec();
        qDebug() <<"insert into table ok";
    }
}

/*
 *????????????????????????
 *????????????????
 *??????????§Ö????????????????????§Ø???????
*/
int LoginDialog::FindTableIsEmpty(QString tablename)
{
    int value = 0;
    QString sql = "select count(*) from "+tablename+";";
    qDebug() <<sql;
    QSqlQuery query;
    query.exec(sql);
    if(query.next())
    {
        value = query.value(0).toInt();
        qDebug() <<"in "<<value;
    }
    qDebug()<<"out "<<value;
    return value;
}

/*
 *???????????????§Ö?????
 *
*/
void LoginDialog::DeleteTableContent(QString tablename)
{
    QString sql ="delete from "+tablename+";";
    qDebug() <<sql;
    QSqlQuery query;
    query.exec(sql);
}

void LoginDialog::on_btnLogin_clicked()
{
    QString UserName = ui->txtUserName->text().trimmed();
    QString UserPwd = ui->txtUserPwd->text().trimmed();
    ui->txtUserPwd->setFocus();
    //????????
    if(ui->radioButtonAdmin->isChecked())
    {
        if(UserPwd.isEmpty()&&UserName.isEmpty())
        {
            myHelper::ShowMessageBoxError(tr("????????????????,??????????!"));
        }
        else
        {
            QSqlTableModel model;
            model.setTable("Staff");
            model.setFilter(QObject::tr("StaffName = '%1' and StaffPassword ='%2'")
                            .arg(UserName).arg(UserPwd));
            model.select();

           // if(model.rowCount() == 1)
            if(true)
            {
                Myapp::LastLoginter = ui->txtUserName->text();
                Myapp::CurrentUserName = Myapp::LastLoginter;
                Myapp::CurrentUserPwd = ui->txtUserPwd->text();
                Myapp::CurrentUserType = tr("?????");
                Myapp::WriteConfig();           //§Õ?????????

                myHelper::MyLoginBlog("logblog","???","?????","?????");     //§Õ???????
                qDebug() <<UserName<<""<<UserPwd;
                //accept();
                mainwidget = new Widget;
                mainwidget->show();
                this->close();
            }
            else
            {
                myHelper::ShowMessageBoxError(tr("????????????????????!"));
                ui->txtUserPwd->clear();
                ui->txtUserPwd->setFocus();
            }
        }
    }
    else if(ui->radioButtonCustomer->isChecked())
    {
        //?????
        if(UserPwd.isEmpty()&&UserName.isEmpty())
        {
            myHelper::ShowMessageBoxError(tr("????????????????,??????????!"));
        }
        else
        {
            QSqlQuery query;
            QString sql = "select customerPassword from customer where CustomerName='"+UserName+"';";
            qDebug() <<sql;
            query.exec(sql);
            query.next();
            QString customePwd = query.value(0).toString();
            if(customePwd == UserPwd)
            {
                Myapp::LastLoginter = ui->txtUserName->text();
                Myapp::CurrentUserName = Myapp::LastLoginter;
                Myapp::CurrentUserPwd = ui->txtUserPwd->text();
                Myapp::CurrentUserType = tr("??????");
                Myapp::WriteConfig();           //§Õ?????????
                myHelper::MyLoginBlog("logblog","?????","???",ui->txtUserName->text());     //§Õ???????
                customewidget->show();
                this->close();
            }
            else
            {
                total++;
                myHelper::ShowMessageBoxError(tr("????????????????????!"));
                ui->txtUserPwd->clear();
                ui->txtUserPwd->setFocus();

                if(total == 3)
                {
                    myHelper::ShowMessageBoxError(tr("????????????????????????????????????????????????"));
                    qApp->quit();
                }
            }

        }
    }
    else
    {
        myHelper::ShowMessageBoxError(tr("?????????????????!"));
    }

}

bool LoginDialog::eventFilter(QObject *obj, QEvent *event)
{
    //???????????,??????????.
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
        {
            this->on_btnLogin_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void LoginDialog::mouseMoveEvent(QMouseEvent *e)
{
    if(mousePressed && (e->buttons()) && Qt::LeftButton)
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void LoginDialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void LoginDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void LoginDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void LoginDialog::on_btnresgister_clicked()
{
    registerdialog = new RegisterDialog();
    registerdialog->exec();
}

void LoginDialog::on_radioButtonCustomer_clicked()
{
    ui->txtUserName->setText("???");
    ui->txtUserPwd->setText("123456");
}

void LoginDialog::on_radioButtonAdmin_clicked()
{
    ui->txtUserName->setText("admin");
    ui->txtUserPwd->setText("admin");
}
