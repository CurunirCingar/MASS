#ifndef DESIGNPROXYMODEL_H
#define DESIGNPROXYMODEL_H

#include <QSize>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <QIdentityProxyModel>
//! \addtogroup TaskListView_imp
//! \{
class DesignProxyModel : public QIdentityProxyModel
{
    QAbstractItemModel *model;
public:
    DesignProxyModel(QAbstractItemModel *model);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
};
//! \}
#endif // DESIGNPROXYMODEL_H
