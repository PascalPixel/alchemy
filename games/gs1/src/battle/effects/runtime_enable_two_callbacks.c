#include "types.h"
#include "scene.h"

extern u8 gRom;
extern u8 gRom2;

void BattleFx_EnableTwoCallbacks(void)
{
    Battle_Check((u32)&gRom);
    Battle_Check((u32)&gRom2);
}
