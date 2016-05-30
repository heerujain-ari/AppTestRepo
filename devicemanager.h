#ifndef __DEVICEMANAGER__
#define __DEVICEMANAGER__

#include <string>
#include <cstdint>

#define OUT
#define IN

namespace goprodfu
{

	// This typedef is for DeviceManager internal consumption and underlying type must not be used.
	// DeviceID must be used instead.
	typedef std::string DeviceID;

	enum DeviceType
	{
		DeviceTypStatus_slingshot,
		DeviceTypStatus_sniper,
		DeviceTypStatus_unknown
	};


    /*struct DeviceInfo
	{
		DeviceID deviceID;
		DeviceType type;
		std::string name;
		std::string serialNumber;
    };*/

    struct DeviceInfo
    {
        std::int32_t deviceID;
        std::string type;
        std::string name;
        std::string serialNumber;
    };


	struct DeviceDetails
	{
		DeviceInfo info;
		std::string firmwareVersion; // field bcdDevice of deviceDescriptor as per usb specifications
	};


	enum Status
	{
		Status_successful,
		Status_inProgress,
		Status_failedUnknownReason,
		Status_deviceNotFound,
		Status_unknownDeviceType,
		Status_fileNotFound,
		Status_filePermissionDenied,
		Status_invalidFile,
		Status_invalidOperation,
		Status_deviceBusy,
		Status_backupComplete,
		Status_backupFailed,
		Status_restoreFailed,
		Status_restoreSuccessful,
		Status_backupUnavailable
	};

	struct OperationStatus {

		Status status;
		uint32_t percentCompleted;
	};


	class DeviceConnectionCallback
	{
	public:
		virtual void Connected(DeviceInfo const &device) = 0;
		virtual void Disconnected(DeviceInfo const & device) = 0;
	};


	class DeviceOperationCallback
	{
	public:

		virtual void Started() = 0;


		virtual void Progress(OperationStatus const & status) = 0;


		virtual void Completed(Status const & status) = 0;
	};

	class DeviceManager {

	private:
		DeviceManager();
		
		DeviceManager* manager;

	public:
	
		DeviceManager (DeviceManager const &) = delete;
		DeviceManager& operator= (DeviceManager const &) = delete;
		~DeviceManager();
        static DeviceManager& getInstance();
		Status registerType(IN DeviceType const & type, IN DeviceConnectionCallback& callback);

		Status getDeviceInfo(IN DeviceID deviceID, OUT DeviceDetails const & details);

		Status takeBackup(IN DeviceID deviceID, IN DeviceOperationCallback& operationCallback);

		Status uploadFirmware(IN DeviceID deviceID, IN std::string file, IN DeviceOperationCallback& operationCallback);

		Status downloadFirmware(IN DeviceID deviceID, IN std::string file, IN DeviceOperationCallback& operationCallback, IN bool mandatoryBackup = false);
	};
}

#endif
