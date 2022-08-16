#include "BLEDevice.h"

using std::uint8_t;

class Bluetooth
{
public:
    Bluetooth();

    void Init();
    void Run();
    void SetVolumePointer(uint8_t *volume);
    void SetPausePointer(bool *pause);

private:
};
