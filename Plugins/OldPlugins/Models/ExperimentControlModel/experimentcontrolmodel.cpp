#include "experimentcontrolmodel.h"

#include <QDebug>

ExperimentControlModel::ExperimentControlModel()
{
    myReferencedPlugin = nullptr;
    openedView = nullptr;
    openedModel = nullptr;
    experimentState = false;
    AvailablePortsModel.setColumnCount(3);
    AvailablePortsModel.setHeaderData(0, Qt::Horizontal, "Port");
    AvailablePortsModel.setHeaderData(1, Qt::Horizontal, "Vendor");
    AvailablePortsModel.setHeaderData(2, Qt::Horizontal, "Device");
}

ExperimentControlModel::~ExperimentControlModel()
{
}


void ExperimentControlModel::SetPluginInfo(PluginInfo *pluginInfo)
{
    this->pluginInfo = pluginInfo;
}

void ExperimentControlModel::OnAllSetup()
{
}

QString ExperimentControlModel::GetLastError()
{
}

void ExperimentControlModel::UpdateDevicesList()
{
    auto devices = myReferencedPlugin->GetAvailableModbusDeviceHandlers();

    if(AvailableDevicesModel.rowCount() == 0 && devices.length() > 0)
        RegisterPacksModel.SetActiveDevice(devices.at(0));
    else if(devices.length() == 0)
        RegisterPacksModel.SetActiveDevice(nullptr);

    AvailableDevicesModel.UpdateDevicesList(devices);
}

void ExperimentControlModel::AddReferencePlugin(PluginInfo *pluginInfo)
{
    switch(pluginInfo->Meta->Type)
    {
        case VIEWPLUGIN:
        {
            relatedViewPlugins.append(pluginInfo);
            qDebug() << "New IViewPlugin added (" << pluginInfo->Meta->Name << ").";
            connect(pluginInfo->Instance, SIGNAL( OnClose(PluginInfo*) ), SLOT( ReferencePluginClosed(PluginInfo*) ));
        } break;

        case MODELPLUGIN:
        {
            relatedModelPlugins.append(pluginInfo);
            qDebug() << "New IModelPlugin added (" << pluginInfo->Meta->Name << ").";
            connect(this, SIGNAL(OnClose(PluginInfo*)), pluginInfo->Instance, SLOT(ReferencePluginClosed(PluginInfo*)));
        } break;

        case COREPLUGIN:
        {
            if(pluginInfo->Meta->InterfaceName == "IMAINMENUMODEL")
            {
                pluginInfo->Plugin.model->AddReferencePlugin(this->pluginInfo);
            }
        } break;

        case DATAMANAGERPLUGIN:
        {
            myReferencedPlugin = qobject_cast<IModbusDeviceDataManager*>(pluginInfo->Instance);

            if(!myReferencedPlugin)
            {
                qDebug() << pluginInfo->Meta->Name << "is not IModbusDeviceDataManager.";
                return;
            }

            qDebug() << "IModbusDeviceDataManager succesfully set.";
            connect(this, SIGNAL(OnClose(PluginInfo*)), pluginInfo->Instance, SLOT(ReferencePluginClosed(PluginInfo*)));
            connect(pluginInfo->Instance, SIGNAL(ErrorOccurred(QString)), this, SIGNAL(ErrorOccurred(QString)));
            connect(pluginInfo->Instance, SIGNAL(ModbusListUpdated()), this, SLOT(UpdateDevicesList()));
        } break;
    }
}

void ExperimentControlModel::ReferencePluginClosed(PluginInfo *pluginInfo)
{
}

bool ExperimentControlModel::Open(IModelPlugin *parent)
{
    qDebug() << "ExperimentControlModel open.";

    if(relatedViewPlugins.count() == 0)
    {
        qDebug() << "!ExperimentControlModel hasn't any views!";
        return false;
    }

    openedView = relatedViewPlugins.first();
    qDebug() << "ExperimentControlModel opens related view " << openedView->Meta->Name;

    if(!openedView->Plugin.view->Open(this))
    {
        qDebug() << "!Can't open first view!";
        openedView = nullptr;
        return false;
    }

    return true;
}

void ExperimentControlModel::Close()
{
    qDebug() << "ExperimentControlModel close.";

    if(openedView != nullptr && !openedView->Plugin.view->Close())
    {
        qDebug() << "View plugin" << openedView->Meta->Name
                 << "not closed, but model closing anyway.";
    }

    if(openedModel != nullptr)
        openedView->Plugin.view->Close();

    openedView = nullptr;
    openedModel = nullptr;
    emit OnClose(pluginInfo);
    emit OnClose();
}

void ExperimentControlModel::ProcessDataInput(QModbusDataUnit::RegisterType dataType, const QVector<quint16> &data)
{
    qDebug() << "KEKS" << dataType << data;
}

//QList<IExperimentControlModel::IExperimentSetup *> ExperimentControlModel::GetAvailableExperimentSetups()
//{
//}

void ExperimentControlModel::StartExperiment()
{
    experimentStartTime = QDateTime::currentDateTime();
    RegisterPacksModel.CleanStoredData();
    RegisterPacksModel.SetStartTime(experimentStartTime);
    setProperty("ExperimentState", true);
}

void ExperimentControlModel::StopExperiment()
{
    setProperty("ExperimentState", false);
}

QAbstractItemModel *ExperimentControlModel::GetAvailablePorts()
{
    UpdateAvailablePorts();
    return &AvailablePortsModel;
}

void ExperimentControlModel::UpdateAvailablePorts()
{
    auto ports = myReferencedPlugin->GetAvailablePorts();
    AvailablePortsModel.clear();

    foreach (auto port, ports)
    {
        QList<QStandardItem *> itemList =
        {
            new QStandardItem(port.portName),
            new QStandardItem(QString::number(port.vendorId)),
            new QStandardItem(QString::number(port.productId))
        };
        AvailablePortsModel.appendRow(itemList);
    }
}

bool ExperimentControlModel::OpenPort(int modelIndex, IModbusDeviceDataManager::ConnectionSettings connectionSettings)
{
    myReferencedPlugin->OpenPort(AvailablePortsModel.item(modelIndex, 0)->text(), connectionSettings);
}

void ExperimentControlModel::ScanForDevice(int deviceId)
{
    myReferencedPlugin->ScanForDevice(deviceId);
}

QAbstractItemModel *ExperimentControlModel::GetAvailableModbusDeviceHandlers()
{
    return &AvailableDevicesModel;
}

QAbstractItemModel *ExperimentControlModel::GetAvailableModbusDeviceNames()
{
    return AvailableDevicesModel.GetDeviceNamesModel();
}

void ExperimentControlModel::ClosePort(int modelIndex)
{
}


QAbstractItemModel *ExperimentControlModel::GetRegisterPacks()
{
    return &RegisterPacksModel;
}

void ExperimentControlModel::SetDeviceForSetup(QString deviceName)
{
    auto *device = AvailableDevicesModel.GetDeviceByName(deviceName);
    RegisterPacksModel.SetActiveDevice(device);
}

QList<QLineSeries *> ExperimentControlModel::GetRegistersLineSeries()
{
    return RegisterPacksModel.GetDataLineSeries();
}

void ExperimentControlModel::AddRegisterPack(RegstersPack pack)
{
    RegisterPacksModel.AddRegisterPack(pack);
}

bool ExperimentControlModel::GetExperimentState()
{
    return experimentState;
}

QDateTime ExperimentControlModel::GetExperimentStartTime()
{
    return experimentStartTime;
}


bool ExperimentControlModel::SaveExperimentData(QString filename)
{
    QFile dataFile(filename);

    if(!dataFile.open(QIODevice::WriteOnly))
    {
        qCritical() << "Can't create file with filename" << filename << dataFile.errorString();
        return false;
    }

    QTextStream stream(&dataFile);
    stream << GetExperimentStartTime().toString("hh:mm:ss.zzz") << "\r\n";
    auto lineSeries = GetRegistersLineSeries();

    foreach (auto line, lineSeries)
    {
        auto data = line->pointsVector();

        for(int i = 0; i < data.length(); ++i)
        {
            stream << data[i].x() << ";" << data[i].y() << "\r\n";
        }
    }
}