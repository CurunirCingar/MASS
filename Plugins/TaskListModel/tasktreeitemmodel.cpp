#include "tasktreeitemmodel.h"


TaskTreeItemModel::TaskTreeItemModel(QString tableName,
                                     ITaskDataManagerPlugin* dataManager,
                                     QList<ITaskDataManagerPlugin::TaskInfo> &data,
                                     QObject *parent)
{
    this->tableName = tableName;
    this->dataManager = dataManager;

    QMap<int, TreeItem*> treeItemIdMap;
    QMap<int, QMap<int, TreeItem*>> treeItemParentMap;
    TreeItem *treeItem;
    ITaskDataManagerPlugin::TaskInfo *managerTaskData;
    for(int i = 0; i < data.count(); i++)
    {
        treeItem = new TreeItem();
        managerTaskData = &data[i];

        treeItem->SetId(managerTaskData->id);
        treeItem->SetName(managerTaskData->name);

        treeItemIdMap.insert(managerTaskData->id, treeItem);
        qDebug() << managerTaskData->id << managerTaskData->name << managerTaskData->position << managerTaskData->parent;

        if(data[i].parent == -1)
            rootItem = treeItem;
        else
        {
            qDebug() << "Child" << managerTaskData->parent << managerTaskData->position;
            treeItemParentMap[managerTaskData->parent].insert(managerTaskData->position, treeItem);
        }
    }
    QList<int> keys = treeItemParentMap.keys();
    for(int i = 0; i < keys.count(); i++)
    {
        TreeItem* parent = treeItemIdMap[keys[i]];
        qDebug() << "Childs" << keys[i] << treeItemParentMap[keys[i]].count() << "|";
        QList<TreeItem*> childItemsList = treeItemParentMap[keys[i]].values();
        for(int j = 0; j < childItemsList.count(); j++)
        {
           childItemsList[j]->parentItem = parent;
           qDebug() << "P" << childItemsList[j]->GetId() << parent->GetId();
        }
        parent->SetChilds(childItemsList);
    }
}

TaskTreeItemModel::~TaskTreeItemModel()
{
    delete rootItem;
}

QVariant TaskTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->GetName();
}

Qt::ItemFlags TaskTreeItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TaskTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->GetName();

    return QVariant();
}

QModelIndex TaskTreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->GetChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TaskTreeItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->GetRow(), 0, parentItem);
}

int TaskTreeItemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->ChildCount();
}

int TaskTreeItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
             return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
         else
             return rootItem->columnCount();
}

ITaskDataManagerPlugin::TaskInfo TaskTreeItemModel::ConvertToManagerTaskInfo(TreeItem* item)
{
    ITaskDataManagerPlugin::TaskInfo managerStruct;
    //Set id
    managerStruct.id = item->GetId().toInt();
    // Set name
    managerStruct.name = item->GetName().toString();
    // Set parent
    managerStruct.parent = item->parentItem ? item->parentItem->GetId().toInt() : -1;
    // Set position
    if(item->parentItem)
        managerStruct.position = item->parentItem->GetChildPosition(item);
    else
        managerStruct.position = 0;
    return managerStruct;
}

void TaskTreeItemModel::DeleteFromManagerRecursive(TreeItem *task)
{
    if(task->ChildCount() != 0)
        for(int i = 0; i < task->ChildCount(); i++)
            DeleteFromManagerRecursive(task->GetChild(i));

    dataManager->DeleteTask(tableName, task->GetId().toInt());
}

//bool TaskTreeItemModel::AddTask(TaskInfo *taskParent, TaskInfo taskData)
//{
//    qDebug() << "Add task";
//    TaskInfo *newTask = new TaskInfo();
//    newTask->name = taskData.name;
//    qDebug() << taskParent->childTasks;
//    if(taskParent != NULL)
//        taskParent->childTasks.append(newTask);
//    newTask->parent = taskParent;

//    ITaskDataManagerPlugin::TaskInfo managerTask = ConvertToManagerTaskInfo(*newTask);
//    qDebug() << managerTask.id << managerTask.name << managerTask.parent << managerTask.position;
//    newTask->id = dataManager->AddTask(tableName, managerTask);
//}

//bool TaskTreeItemModel::EditTask(ITaskTreeModel::TaskInfo *task, ITaskTreeModel::TaskInfo taskData)
//{
//    task->id = taskData.id;
//    task->name = taskData.name;

//    ITaskDataManagerPlugin::TaskInfo managerTask = ConvertToManagerTaskInfo(*task);
//    dataManager->EditTask(tableName, managerTask);
//}

//bool TaskTreeItemModel::DeleteTask(ITaskTreeModel::TaskInfo *task)
//{
//    if(!task)
//        return false;
//    if(task->parent)
//        task->parent->childTasks.removeOne(task);
//    DeleteTaskRecursive(task);

//    dataManager->DeleteTask(tableName, task->id);
//}