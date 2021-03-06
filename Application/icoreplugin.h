#ifndef ICOREPLUGIN_H
#define ICOREPLUGIN_H

#include "iapplication.h"

//!  \defgroup MainApp MainApp
//!  \defgroup Plugins Plugins

//! \addtogroup MainApp ICore
//! \{

//! \brief Interface for core plugin.
//! \details This interface represents most significant plugin in whole system. If plugin implements this
//! interface it should take care of whole system plugins interactions.
//! When PluginLoader class loads plugins from directory it searchs the plugin which
//! inherits from ICorePlugin. If it find one - it will send all loaded plugins to it by using AddPlugin()
//! method.
class ICore
{
public:
    //! \brief Starts plugin work. Some kind of 'start' signal.
    //! \param This will be parent widget for all widgets in app.
    virtual void coreInit(IApplication* app) = 0;

    //! \brief Starts plugin work. Some kind of 'start' signal.
    //! \param This will be parent widget for all widgets in app.
    virtual bool coreFini() = 0;
protected:
    virtual ~ICore() {}
};
Q_DECLARE_INTERFACE(ICore, "ICore")
//! \}
#endif // ICOREPLUGIN_H

