#include "TYPES.H"
#include "SCENE.H"

extern u8 gRom;
extern u8 gRom2;

void BattleFx_EnableTwoCallbacks(void)
{
    Battle_Check((u32)&gRom);
    Battle_Check((u32)&gRom2);
}
