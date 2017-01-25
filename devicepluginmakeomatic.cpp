/*!
	\page makeomatic.html
	\title Make-O-Matic
	\brief Plugin for Make-O-Matic.

	\ingroup plugins
	\ingroup makeomatic-plugins

	This plugin allows controlling Make-O-Matic.

	\chapter Plugin properties
	Following JSON file contains the definition and the description of all available \l{DeviceClass}{DeviceClasses}
	and \l{Vendor}{Vendors} of this \l{DevicePlugin}.

	For more details on how to read this JSON file please check out the documentation for \l{The plugin JSON File}.

	\quotefile plugins/deviceplugins/multisensor/devicepluginmultisensor.json
*/

#include <QSharedPointer>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>
#include "plugininfo.h"
#include "devicemanager.h"
#include "devicepluginmakeomatic.h"

/* The constructor of this device plugin. */
DevicePluginMakeOMatic::DevicePluginMakeOMatic()
{

}

/* This method will be called from the devicemanager to get
 * information about this plugin which device resource will be needed.
 */
DeviceManager::HardwareResources DevicePluginMakeOMatic::requiredHardware() const
{
    return DeviceManager::HardwareResourceNone;
}


/* This method will be called from the devicemanager while he
 * is setting up a new device. Here the developer has the chance to
 * perform the setup on the actual device and report the result.
 */
DeviceManager::DeviceSetupStatus DevicePluginMakeOMatic::setupDevice(Device *device)
{
    qCDebug(dcMakeOMatic) << "Setting up" << device->name() << device->params();

    if (device->deviceClassId() == gloveDeviceClassId) {
        auto leftAddress = QBluetoothAddress(configValue(leftMACParamTypeId).toString());
        auto rightAddress = QBluetoothAddress(configValue(rightMACParamTypeId).toString());
        auto name = device->paramValue(gloveNameParamTypeId).toString();
        auto leftInfo = QBluetoothDeviceInfo(leftAddress, name, 0);
        auto rightInfo = QBluetoothDeviceInfo(rightAddress, name, 0);
        
        QSharedPointer<Glove> glove{new Glove(leftInfo, rightInfo, this)};
        m_devices.insert(glove, device);

        glove->connect();

        return DeviceManager::DeviceSetupStatusSuccess;
    }
    return DeviceManager::DeviceSetupStatusFailure;
}

void DevicePluginMakeOMatic::deviceRemoved(Device *device)
{
    if (!m_devices.values().contains(device))
        return;

    auto glove = m_devices.key(device);

    m_devices.remove(glove);
}

