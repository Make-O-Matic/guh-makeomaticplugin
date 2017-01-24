#ifndef DEVICEPLUGINMAKEOMATIC_H
#define DEVICEPLUGINMAKEOMATIC_H

#include <QHash>
#include <QPointer>
#include <QSharedPointer>
#include "plugin/deviceplugin.h"
#include "devicemanager.h"
//#include "bt/"
#include "glove.h"

class DevicePluginMakeOMatic : public DevicePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "guru.guh.DevicePlugin" FILE "devicepluginmakeomatic.json")
    Q_INTERFACES(DevicePlugin)

public:
    explicit DevicePluginMakeOMatic();
    
    DeviceManager::HardwareResources requiredHardware() const override;
    DeviceManager::DeviceSetupStatus setupDevice(Device *device) override;
    void deviceRemoved(Device *device) override;

private:
    QHash<QSharedPointer<Glove>, QPointer<Device>> m_devices;
};

#endif // DEVICEPLUGINMAKEOMATIC_H
