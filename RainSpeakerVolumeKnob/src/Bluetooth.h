#include <cstdint>
#include <BLECharacteristic.h>

using std::uint8_t;

class Bluetooth
{
public:
    Bluetooth();

    void Init();
    void SetPosition(uint8_t position);
    void SetButton(uint8_t button);

private:
    BLECharacteristic *pCharacteristicEnc;
    BLECharacteristic *pCharacteristicBtn;
};
