#include "tablehandler.h"

TableHandler::TableHandler(IDataBaseSourcePlugin *dataSource, QString tableName)
{
    this->dataSource = dataSource;
    this->tableName = tableName;

    dataBaseTypesNames.insert(QVariant::Int,    "INTEGER");
    dataBaseTypesNames.insert(QVariant::String, "VARCHAR");

    coreTableStruct.insert("id", QVariant::Int);

    CombineWholeTableStruct();

    if(tableName == "")
        isCreated = false;
    else
        CreateTable();
}

bool TableHandler::CreateTable()
{
    // Is name valid
    qDebug() << "Create new table" << tableName;
    if(tableName == NULL || tableName == "")
    {
        qDebug() << "Tree name is empty!";
        return false;
    }
    tableName = tableName.toLower();

    // Is DataSource set
    if(!dataSource)
    {
        qDebug() << "DBManager isnt set!";
        return false;
    }

    // Create query
    QString queryStr = QString("CREATE TABLE IF NOT EXISTS %1 (%2)")
            .arg(tableName)
            .arg(GetHeaderString(coreTableStruct));
    qDebug() << "Creating table" << queryStr;
    QSqlQuery query = dataSource->ExecuteQuery(queryStr);

    // Is table has right structure
//    if(!IsTableRightStructure(tableName))
//    {
//        qDebug() << "Table" << tableName << "has wrong structure";
//        return false;
//    }

    isCreated = true;
    return true;
}

bool TableHandler::SetRelation(QString relationName, TableStructMap fields)
{
    qDebug() << "SetRelation";
    if(relationName == "")
    {
        qDebug() << "Relation name is empty!";
        return false;
    }

    fields.insert("id", QVariant::Int);
    QString dataFields = GetHeaderString(fields, true);
    if(dataFields == "")
    {
        qDebug() << "Can't create relation!";
        return false;
    }

    relationName = relationName.toLower();
    QString queryStr = QString("CREATE TABLE IF NOT EXISTS r_%1_%2 (%3)")
            .arg(tableName)
            .arg(relationName)
            .arg(dataFields);
    QSqlQuery queryResult = dataSource->ExecuteQuery(queryStr);

    relationTableStructs.insert(relationName, fields);
    return true;
}

bool TableHandler::DeleteRelation(QString relationName)
{
    qDebug() << "DeleteRelation";
    relationName = relationName.toLower();
    QString queryStr = QString("DROP TABLE IF EXISTS  %1_%2")
            .arg(tableName)
            .arg(relationName);
    QSqlQuery queryResult = dataSource->ExecuteQuery(queryStr);
    relationTableStructs.remove(relationName);
    CombineWholeTableStruct();
    return true;
}

QList<IExtendableDataBaseManagerPlugin::TreeItemInfo> TableHandler::GetData()
{
    QString queryStr = "";
    queryStr = QString("SELECT * FROM %1 ").arg(tableName);
    //left outer join on
    QStringList joinTables = relationTableStructs.keys();
    for(int i = 0; i < joinTables.count(); i++)
    {
        queryStr.append(QString(" LEFT JOIN r_%1_%2 ON %1.id = r_%1_%2.id ")
                        .arg(tableName)
                        .arg(joinTables[i]));
    }
    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
    QList<TreeItemInfo> taskTree;
    TreeItemInfo buf;

    QString bufStr;
    while (query.next()) {
        bufStr = "";
        //for(int i = 0; i < query.; ++i)
        bufStr.append(query.value(0).toString() + " ");
        bufStr.append(query.value(1).toString() + " ");
        bufStr.append(query.value(2).toString() + " ");
        bufStr.append(query.value(3).toString() + " ");
        bufStr.append(query.value(4).toString() + " ");
        bufStr.append(query.value(5).toString() + " ");
        qDebug() << bufStr;
//        for(int i = 3; i < query.size(); i++)
//        {
//            buf.data.append(query.value(i));
//        }
//        taskTree.append(buf);
    }
    return QList<TreeItemInfo>();
}

int TableHandler::AddItem(TreeItemInfo item)
{
    qDebug() << "AddItem";

    QString queryStr = QString("INSERT INTO %1 (id) VALUES (NULL)").arg(tableName);
    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
    int lastId = query.lastInsertId().toInt();

    QStringList joinTables = item.dataChunks.keys();
    for(int i = 0; i < joinTables.count(); i++)
    {
        QString valuesString = GetValuesString(relationTableStructs[joinTables[i]], lastId, item.dataChunks[joinTables[i]]);
        if(valuesString != "")
        {
            queryStr = "";
            queryStr.append(QString("INSERT INTO r_%1_%2 %3")
                            .arg(tableName)
                            .arg(joinTables[i])
                            .arg(valuesString)
                            );
            dataSource->ExecuteQuery(queryStr);
        }
        ++i;
    }

    //QStringList relationsNames = relationsMap[treeName];

    return lastId;
}

bool TableHandler::EditItem(TreeItemInfo item)
{
//    treeName = treeName.toLower();
//    QString queryStr = QString("update %1 set name='%3', parent=%4, position=%5 where id=%2")
//            .arg(treeName)
//            .arg(item.id)
//            .arg(item.parent)
//            .arg(item.position)
//            ;
//    qDebug() << "Edit Task" << queryStr;
//    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
//    if(query.lastError().text() != "")
//    {
//        lastError = query.lastError().text();
//        return false;
//    }
//    return true;
}

bool TableHandler::DeleteItem(int id)
{
//    treeName = treeName.toLower();
//    QString queryStr = QString("delete from %1 where id=%2").arg(treeName).arg(id);
//    qDebug() << "Delete Task" << queryStr;
//    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
//    if(query.lastError().text() != "")
//    {
//        lastError = query.lastError().text();
//        return false;
//    }
//    return true;
}

TableHandler::TableStructMap TableHandler::GetHeader()
{
    return wholeTableStruct;
}

QString TableHandler::GetHeaderString(TableStructMap &tableStruct, bool createRelation)
{
    QString structStr = "";
    TableStructMap::Iterator i = tableStruct.begin();
    TableStructMap::Iterator lastElement = --tableStruct.end();
    while(i != tableStruct.end())
    {
        QString typeNameString = dataBaseTypesNames.contains(i.value()) ? dataBaseTypesNames[i.value()] : QVariant::typeToName(i.value());
        structStr.append(QString("%1 %2").arg(i.key()).arg(typeNameString));
        if(i.key() == "id")
        {
            QString idAppendix = createRelation ?
                        QString(" REFERENCES %1(id) ON DELETE CASCADE").arg(tableName) :
                        " PRIMARY KEY AUTOINCREMENT";
            structStr.append(idAppendix);
        }
        if(i != lastElement) structStr.append(",");
        ++i;
    }
    return structStr;
}

QString TableHandler::GetValuesString(TableStructMap &tableStruct, int id, QVector<QVariant> &itemData)
{
    if(tableStruct.count() != itemData.count())
    {
        qDebug() << "Wrong size" << tableStruct.count() << itemData.count();
        return QString();
    }

    QString fieldNamesStr = "(";
    QString valuesStr = "(";
    TableStructMap::Iterator structIter = tableStruct.begin();
    TableStructMap::Iterator lastElement = --tableStruct.end();
    QVector<QVariant>::Iterator dataIter = itemData.begin();
    while(structIter != tableStruct.end())
    {
        fieldNamesStr.append( QString("%1").arg(structIter.key()) );
        if(structIter.key() == "id")
        {
            valuesStr.append(QString::number(id));
        }
        else
        {
            QString buf;
            if(dataIter->type() == QVariant::String)
                buf = QString("'%1'").arg(dataIter->toString());
            else
                buf = dataIter->toString();
            valuesStr.append(buf);
        }

        if(structIter != lastElement)
        {
            fieldNamesStr.append(",");
            valuesStr.append(",");
        }
        else
        {
            fieldNamesStr.append(")");
            valuesStr.append(")");
        }
        ++structIter;
        ++dataIter;
    }
    return QString("%1 VALUES %2").arg(fieldNamesStr).arg(valuesStr);
}

bool TableHandler::IsTableExists(QString tableName)
{
    // BUG: Not work
    QString queryStr = QString("pragma table_info(%1)").arg(tableName);
    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
    return query.size() > 0;
}

void TableHandler::CombineWholeTableStruct()
{
    wholeTableStruct.clear();
    wholeTableStruct.unite(coreTableStruct);

    QMap<QString, TableStructMap>::Iterator i = relationTableStructs.begin();
    while(i != relationTableStructs.end())
    {
        wholeTableStruct.unite(i.value());
        ++i;
    }
}

//bool TableHandler::IsTableRightStructure(QString tableName)
//{
//    QString queryStr = QString("pragma table_info(%1)").arg(tableName);
//    QSqlQuery query = dataSource->ExecuteQuery(queryStr);
//    QString name;
//    QVariant::Type type;

//    for(int i = 0; i < coreTableStruct.count(); i++)
//    {
//        if(!query.next())
//        {
//            qDebug() << "Too few records!";
//            return false;
//        }
//        qDebug() << query.value(0).toInt() << ": " << query.value(1).toString() << ": " << query.value(2).toString();
//        name = query.value(1).toString();
//        type = query.value(2).type();
//        if(coreTableStruct[i].name != name || coreTableStruct[i].type != type)
//        {
//            qDebug() << name << type << "cant exist in right table structure";
//            return false;
//        }
//    }
//    return true;
//}