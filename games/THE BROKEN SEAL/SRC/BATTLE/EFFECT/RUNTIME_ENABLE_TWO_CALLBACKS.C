#include "TYPES.H"
#include "SCENE.H"

extern u8 gRom;
extern u8 gRom2;

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)&gRom);
    Scheduler_EnableCallbacks((u32)&gRom2);
}
