#ifndef TASKLISTVIEW_H
#define TASKLISTVIEW_H

#include <QApplication>
#include <QDebug>
#include <QLayout>
#include <QAbstractProxyModel>
#include <QPainter>
#include <QAbstractItemModel>
#include <QResizeEvent>

#include "../../Common/plugin_base.h"
#include "../../Interfaces/itasktreemodel.h"

#include "designproxymodel.h"
#include "addform.h"
#include "mytreeview.h"

//! \defgroup TaskListView
//!     \ingroup MainMenuPlugin_rel_v
//! \defgroup TaskListView_imp Implementation
//!     \ingroup TaskListView

//! \addtogroup TaskListView_imp
//! \{
class TaskListView : public PluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "TimeKeeper.Module.Test" FILE "PluginMeta.json")
    Q_INTERFACES(IPlugin)

public:
    TaskListView(QWidget *parent = nullptr);
    ~TaskListView();

    // PluginBase interface
protected:
    virtual void onAllReferencesSet() override;

private:
    ITaskTreeModel *myModel;

    QIdentityProxyModel *proxyModel;
    QAbstractItemModel *taskTree;

public slots:
    void OpenTaskEditor(int id);
    void resizeEvent(QResizeEvent *event);

private:
    AddForm *addForm;
    QAbstractItemModel *model;
    QTreeView *myTreeView;
    bool expandFlag;
    const QModelIndex *currentModelIndex;

private slots:
    void OnAddFormClosed();

    void on_buttonAdd_clicked();
    void on_buttonDelete_clicked();
    void onTreeViewPressed(const QModelIndex &index);
    void on_buttonEdit_clicked();
    void onTreeViewClicked(const QModelIndex &index);
    void on_buttonExit_clicked();


    // PluginBase interface
public:
    virtual void onAllReferencesReady() override;
};
//! \}
#endif // TASKLISTVIEW_H
