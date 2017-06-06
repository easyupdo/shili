/***************************************************************
 *Copyright(c) 2014-2015 Company By LiQi
 *All rights reserved.
 *�ļ����ƣ����ݿ��Ĵ����ļ�
 *��Ҫ�������������ݿ⣬���ݱ�
 *
 *��ǰ�汾��V1.0
 *���ߣ�Kelvin Li
 *�������ڣ�2014/12
 *˵����Email:creazylq@163.com
*****************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>

/*
 *�������ܣ����������ﲢ����
 *˵�����˺������Խ����������ݿ⣬�����Ӳ��ϣ����������Ĵ�����ʾ
*/
static bool createConnection()
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL"); //����һ�����ݿ�

    db.setHostName("127.0.0.1");                            //���ص�ַ����ҪԶ�����ӣ�������Ӧ�ĵ�ַ��
    db.setPort(3306);                                       //���ö˿ں�
    db.setDatabaseName("hotel");                            //�������ݿ�������
    db.setUserName("root");                                 //�����û�����
    db.setPassword("123456");                               //��������
    if(!db.open())
    {
        qCritical("Can't open database: %s(%s)",
                  db.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());

        return false;
    }
    return true;
}

static bool closeConnection()
{
    QSqlDatabase::database().close();
    return 1;
}

#endif // CONNECTION_H
