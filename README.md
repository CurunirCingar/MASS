# MASS - Multi-Application Support System

#### The main idea of this application is to build a whole application in a modular way - all the application functionality is provided by composing different subsets of plugins, where each plugin should have responsibility over a [s](https://en.wikipedia.org/wiki/Single_responsibility_principle)ingle part of the functionality.
## How it works
1. When main application starts it loads all plugins from folder "/Application/Plugins".

2. Among all loaded plugins it searches plugin, that inherits interface [ICorePlugin](/Application/icoreplugin.h) ([doc](https://curunircingar.github.io/MASS/class_i_core_plugin.html)). 
That's how it looks:
```cpp
class ICorePlugin
{
public:
    virtual void coreInit(IApplication* app) = 0;
    virtual bool coreFini() = 0;
protected:
    virtual ~ICorePlugin() {}
};
```

3. If main application finds plugin with ICorePlugin interface - it will pass instance that inherits [IApplication](/Application/iapplication.h) interface:
```cpp
class IApplication
{
public:
    virtual QWidget* getParentWidget() = 0;
    virtual QWeakPointer<IPluginHandler> getCorePlugin() = 0;
    virtual QVector<QWeakPointer<IPluginHandler>> getPlugins() = 0;
    virtual QWeakPointer<IPluginHandler> makePluginHandler(QString path) = 0;
};
```
to coreInit() method. After that all application behavior controlled by plugins only.

## Development process
All plugins (including core plugin) contains in another repo: https://github.com/CurunirCingar/MASS-Plugins. It's divided from this repo because it's possible to use any plugins set developed by you or any other developer on planet Earth. But if you want to read more about already implemented plugins set and how all this stuff works, visit repo mentioned above.

If you want to read more about main application go here: https://curunircingar.github.io/MASS/.  

## Considerable plugins implementation aspect
All plugins are loaded using [QPluginLoader](http://doc.qt.io/qt-5/qpluginloader.html) and should contain [plugin specific macroses](http://doc.qt.io/qt-5/plugins-howto.html).
If you're using QtCreator then you can use custom wizard for generating solutions for new plugins - [link](https://github.com/CurunirCingar/MASS/tree/master/Resources/QtCreator%20wizard).
