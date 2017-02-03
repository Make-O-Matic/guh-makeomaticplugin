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
#include <QVariant>
#include "plugininfo.h"
#include "devicemanager.h"
#include "glove.h"
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
    qCDebug(dcMakeOMatic) << "Setting up" << device->name() << device->params()
                          << configuration();

    if (device->deviceClassId() == gloveDeviceClassId) {
        QSharedPointer<Glove> glove{new Glove(device->name(),
                                              configValue(leftMACParamTypeId).toString(),
                                              configValue(rightMACParamTypeId).toString(),
                                              [device](){ return device->stateValue(recordingStateTypeId).toBool(); }, this)};
        connect(glove.data(), &Glove::connectionChanged, this,
                [device, this](QVariant state){ setConnectedState(device, state); });

        m_devices.insert(glove, device);

        glove->connectDevice();

        return DeviceManager::DeviceSetupStatusAsync;
    }
    return DeviceManager::DeviceSetupStatusFailure;
}

DeviceManager::DeviceError DevicePluginMakeOMatic::executeAction(Device *device, const Action &action)
{
    if (device->deviceClassId() == gloveDeviceClassId) {
        if (action.actionTypeId() == recordingActionTypeId) {
            device->setStateValue(recordingStateTypeId, action.param(recordingStateParamTypeId).value().toBool());
//db
            return DeviceManager::DeviceErrorNoError;
        } else if (action.actionTypeId() == mutationActionTypeId) {

        }
    }
    return DeviceManager::DeviceErrorNoError;
}

void DevicePluginMakeOMatic::deviceRemoved(Device *device)
{
    if (!m_devices.values().contains(device))
        return;

    auto glove = m_devices.key(device);

    m_devices.remove(glove);
}

void DevicePluginMakeOMatic::setConnectedState(Device *device, QVariant state)
{
    device->setStateValue(connectedStateTypeId, state);
    qCDebug(dcMakeOMatic) << state.toString() << "state";
    if (state.toString() == "both" && !device->setupComplete())
        emit deviceSetupFinished(device, DeviceManager::DeviceSetupStatusSuccess);
}

