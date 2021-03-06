/*****************************************************************************
*                                                                            *
*  PrimeSense Sensor 5.x Alpha                                               *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of PrimeSense Sensor                                    *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#ifndef __XN_SENSOR_PRODUCTION_NODE_H__
#define __XN_SENSOR_PRODUCTION_NODE_H__

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnModuleCppInterface.h>
#include "XnSensor.h"
#include <XnHashT.h>
#include "XnSensorClient.h"

class XnMultiPropChangedHandler; // forward declaration

//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
// disable the "inherits via dominance" warning. This is exactly what we want.
#pragma warning (push)
#pragma warning (disable: 4250)

class XnSensorProductionNode : 
	virtual public xn::ModuleProductionNode,
	virtual public xn::ModuleLockAwareInterface,
	virtual public xn::ModuleExtendedSerializationInterface
{
public:
	XnSensorProductionNode(xn::Context& context, const XnChar* strInstanceName, XnDeviceBase* pSensor, const XnChar* strModuleName);
	virtual ~XnSensorProductionNode();

	// Production Node
	XnBool IsCapabilitySupported(const XnChar* strCapabilityName);
	XnStatus SetIntProperty(const XnChar* strName, XnUInt64 nValue);
	XnStatus SetRealProperty(const XnChar* strName, XnDouble dValue);
	XnStatus SetStringProperty(const XnChar* strName, const XnChar* strValue);
	XnStatus SetGeneralProperty(const XnChar* strName, XnUInt32 nBufferSize, const void* pBuffer);
	XnStatus GetIntProperty(const XnChar* strName, XnUInt64& nValue) const;
	XnStatus GetRealProperty(const XnChar* strName, XnDouble& dValue) const;
	XnStatus GetStringProperty(const XnChar* strName, XnChar* csValue, XnUInt32 nBufSize) const;
	XnStatus GetGeneralProperty(const XnChar* strName, XnUInt32 nBufferSize, void* pBuffer) const;

	// Lock Aware
	xn::ModuleLockAwareInterface* GetLockAwareInterface() { return this; }
	XnStatus SetLockState(XnBool bLocked);
	XnBool GetLockState();
	XnStatus RegisterToLockChange(XnModuleStateChangedHandler handler, void* pCookie, XnCallbackHandle& hCallback);
	void UnregisterFromLockChange(XnCallbackHandle hCallback);

	// Extended Serialization
	xn::ModuleExtendedSerializationInterface* GetExtendedSerializationInterface() { return this; }
	XnStatus NotifyExState(XnNodeNotifications* pNotifications, void* pCookie);
	// TODO: Add RegisterExNotifications() which register to all props
	void UnregisterExNotifications();

	virtual XnStatus Init() { return XN_STATUS_OK; }

	inline XnDeviceBase* GetSensor() { return m_pSensor; }
	inline const XnChar* GetModuleName() { return m_strModule; }
	inline xn::Context& GetContext() { return m_Context; }
	inline const XnChar* GetInstanceName() { return m_strInstanceName; }

protected:
	virtual void FilterProperties(XnActualPropertiesHash* pHash);
	XnStatus RegisterToProps(XnModuleStateChangedHandler handler, void* pCookie, XnCallbackHandle& hCallback, const XnChar** strNames, const XnChar* strModule = NULL);
	void UnregisterFromProps(XnCallbackHandle hCallback);

	void OnModuleIntPropertyChanged(const XnChar* strModule, const XnChar* strProperty);
	void OnModuleRealPropertyChanged(const XnChar* strModule, const XnChar* strProperty);
	void OnModuleStringPropertyChanged(const XnChar* strModule, const XnChar* strProperty);
	void OnModuleGeneralPropertyChanged(const XnChar* strModule, const XnChar* strProperty);

	static void XN_CALLBACK_TYPE IntPropertyChangedCallback(XnDeviceHandle pDeviceHandle, const XnChar* ModuleName, const XnChar* PropertyName, void* pCookie);
	static void XN_CALLBACK_TYPE RealPropertyChangedCallback(XnDeviceHandle pDeviceHandle, const XnChar* ModuleName, const XnChar* PropertyName, void* pCookie);
	static void XN_CALLBACK_TYPE StringPropertyChangedCallback(XnDeviceHandle pDeviceHandle, const XnChar* ModuleName, const XnChar* PropertyName, void* pCookie);
	static void XN_CALLBACK_TYPE GeneralPropertyChangedCallback(XnDeviceHandle pDeviceHandle, const XnChar* ModuleName, const XnChar* PropertyName, void* pCookie);

	typedef XnHashT<XnMultiPropChangedHandler*, XnMultiPropChangedHandler*> XnMultiPropChangedHandlerHash;

	xn::Context m_Context;
	XnDeviceBase* m_pSensor;
	XnChar m_strInstanceName[XN_MAX_NAME_LENGTH];
	XnChar m_strModule[XN_DEVICE_MAX_STRING_LENGTH];
	XnNodeNotifications* m_pNotifications;
	void* m_pCookie;

	XnMultiPropChangedHandlerHash m_AllHandlers;

	XnCallbackHandle m_hIntProps;
	XnCallbackHandle m_hRealProps;
	XnCallbackHandle m_hStringProps;
	XnCallbackHandle m_hGenProps;
};

#pragma warning (pop)

#endif // __XN_SENSOR_PRODUCTION_NODE_H__