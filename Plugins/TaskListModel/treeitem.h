#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVector>
#include <QVariant>
#include <QDebug>

class TreeItem
{
public:
    TreeItem *parentItem;

    TreeItem();
    TreeItem(TreeItem *parent, TreeItem *copy);
    TreeItem(TreeItem *parent, int id, QString activeChunk, QMap<QString, QVector<QVariant>> dataChunks);
    ~TreeItem();

// Columns
    inline int ColumnCount() { return activeChunk ? activeChunk->count() : 0; }
    inline int GetRow() { return parentItem ? parentItem->GetChildPosition(this) : 0; }

// Childs
    void SetChilds(QList<TreeItem *> childs);
    void AddChild(int row, TreeItem *child);
    void RemoveChild(TreeItem *child);
    void RemoveChildAt(int row);
    inline TreeItem *GetChildAt(int row) { return childItems.count() > row ? childItems[row] : NULL;}
    inline int ChildCount() { return childItems.count(); }
    inline int GetChildPosition(TreeItem *item) { return childItems.indexOf(item); }

// Data
    void SetActiveChunkName(QString &activeChunk);
    QList<QString> GetChunksNames();
    QVector<QVariant> GetChunkData(QString chunkName);
    QVariant GetChunkDataElement(int column);
    void SetChunkData(QString chunkName, QVector<QVariant> data);
    void SetChunkDataElement(int column, QVariant data);
    inline int GetId()              { return itemData.id; }
    inline void SetId(int id)       { itemData.id = id; }

private:
    class ItemData{
    public:
        int id;
        QMap<QString, QVector<QVariant>> dataChunks;
    };

    ItemData itemData;
    QString activeChunkName;
    QVector<QVariant> *activeChunk;
    QList<TreeItem*> childItems;
};

#endif // TREEITEM_H
