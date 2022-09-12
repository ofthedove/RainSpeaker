#include "Bluetooth.h"
#include "Arduino.h"

static BLEUUID serviceUUID("ff149c8e-59bf-497d-a11f-d5af3128a195");
static BLEUUID encCharUUID("be516360-2361-45e2-aa33-a157d48d7bc7");
static BLEUUID btnCharUUID("6ba8bfa1-68f0-44dd-bdff-434016586344");

static BLERemoteCharacteristic *pRemoteCharacteristicEnc;
static BLERemoteCharacteristic *pRemoteCharacteristicBtn;
static BLEAdvertisedDevice *myDevice;
static BLEScan *pBLEScan;
static void (*volumeCallback)(uint8_t vol);

static HeartbeatLed *led = NULL;

static bool scanning = false;
static bool deviceFound = false;
static bool connected = false;

Bluetooth::Bluetooth()
{
}

void OnScanComplete(BLEScanResults results)
{
    scanning = false;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{

    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
        {

            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            deviceFound = true;

            // Scan->stop() does not trigger OnComplete callback, must call manually
            OnScanComplete(BLEScanResults());
        }
    }
};

void Bluetooth::Init(HeartbeatLed *statusLed)
{
    led = statusLed;

    Serial.println("Starting Arduino BLE Client application...");
    BLEDevice::init("");

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);

    scanning = true;
    pBLEScan->start(5, OnScanComplete, false);

    Serial.println("Finished scanning");
}

class MyClientCallback : public BLEClientCallbacks
{
    void onConnect(BLEClient *pclient)
    {
        connected = true;
        Serial.println("onConnect");
        led->SetPattern(HeartbeatLedPattern::solidOn);
    }

    void onDisconnect(BLEClient *pclient)
    {
        connected = false;
        deviceFound = false;
        Serial.println("onDisconnect");
        led->SetPattern(HeartbeatLedPattern::blink500);
    }
};

static void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    if (volumeCallback != NULL)
    {
        volumeCallback(*pData);
    }
}

bool connectToServer()
{
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicEnc = pRemoteService->getCharacteristic(encCharUUID);
    if (pRemoteCharacteristicEnc == nullptr)
    {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(encCharUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if (pRemoteCharacteristicEnc->canRead())
    {
        uint8_t value = pRemoteCharacteristicEnc->readUInt8();
        Serial.print("The characteristic value was: ");
        Serial.println(value);
    }

    if (pRemoteCharacteristicEnc->canNotify())
        pRemoteCharacteristicEnc->registerForNotify(notifyCallback);

    return true;
}

void Bluetooth::Run()
{
    if (!scanning && !connected)
    {
        if (deviceFound)
        {
            connectToServer();
        }
        else
        {
            scanning = true;
            pBLEScan->start(5, OnScanComplete, false);
        }
    }
}

void Bluetooth::SetVolumeCallback(void (*callback)(uint8_t vol))
{
    volumeCallback = callback;
}

void Bluetooth::SetPausePointer(bool *pause)
{
}
