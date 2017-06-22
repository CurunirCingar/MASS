#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include <QWidget>
#include <QDebug>
#include <QMap>
#include <QApplication>
#include <QJsonObject>

#include "imainmenumodule.h"
#include "pluginlinker.h"

//! \ingroup MainMenuPlugin_imp
//! @{
class MainMenuModelPlugin : public QObject, IMainMenuPluginModel, IRootModelPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "TimeKeeper.Module.Test" FILE "PluginMeta.json")
    Q_INTERFACES(IModelPlugin IRootModelPlugin IMainMenuPluginModel)

public:
    MainMenuModelPlugin();
    ~MainMenuModelPlugin();

    void AddPlugin(QObject *instance, QJsonObject *meta) override;
    void Open(QWidget *parentWidget) override;

    void OnAllSetup() override;
    QString GetLastError() override;

    void AddDataManager(QObject *dataManager) override;
    void AddParentModel(QObject *model, MetaInfo *meta) override;
    void AddChildModel(IModelPlugin *model, MetaInfo *meta) override;
    void AddView(IViewPlugin *view, MetaInfo *meta) override;

    bool Open(IModelPlugin *parent, QWidget *parentWidget) override;
    bool CloseFromView(IViewPlugin *view) override;
    void ChildSelfClosed(IModelPlugin *child) override;

    MenuItem *GetRootMenuItem() override;
    void RunItem(MenuItem *item, MetaInfo *viewMeta) override;

private:
    QList< PluginInfo<IModelPlugin> > childModels;
    QList< PluginInfo<IViewPlugin> > views;

    QWidget *parentWidget;
    int activeViewId;
    int activeModelId;

    PluginLinker pluginLinker;
    MenuItem *rootMenuItem;
};
//! @}
#endif // MAINMENUMODULE_H
