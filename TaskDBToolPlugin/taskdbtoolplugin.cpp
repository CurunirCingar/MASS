#include "taskdbtoolplugin.h"


TaskDBToolPlugin::TaskDBToolPlugin()
{
    DBManager = NULL;
}

bool TaskDBToolPlugin::SetDBManager(IDBManagerPlugin *DBManager)
{
    if(this->DBManager)
        return false;
    this->DBManager = DBManager;
    return true;
}

bool TaskDBToolPlugin::CreateNewTask()
{
    QSqlQuery query;
    query.exec("create table mapping (id int, name varchar)");
    query.exec("insert into mapping values (1, 'AAA')");
    query.exec("insert into mapping values (2, 'BBB')");
    query.exec("insert into mapping values (3, 'CCC')");
    query.exec("insert into mapping values (4, 'DDD')");
    query.exec("insert into mapping values (5, 'EEE')");
    query.exec("insert into mapping values (6, 'FFF')");
    query.exec("insert into mapping values (7, 'GGG')");
    query.exec("select id, name from mapping");
    while (query.next()) {
        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
    }
    qDebug() << "----------" << endl;
    query.exec("update mapping set name='ZZZ' where id=1");
    query.exec("select id, name from mapping");
    while (query.next()) {
        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
    }
    qDebug() << "----------" << endl;
    //query.exec("delete from mapping where id=4");
    //query.exec("select id, name from mapping");
    query.exec("pragma table_info(mapping)");
    while (query.next()) {
        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString();
    }
    query.exec("drop table mapping");
    return true;
}
