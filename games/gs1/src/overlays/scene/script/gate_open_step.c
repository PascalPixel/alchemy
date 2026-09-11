#include "types.h"
#include "scene.h"

extern u8 gOv[];
extern u8 gWork[];

void Scene_RunGate(void)
{
    Script_Do(0x144);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    if (Script_Check(0x814) != 0) {
        s32 zero = 0;
        *(volatile s32 *)gOv = zero;
        Script_Run(0x2009ac9, 0xc80);
    }
    if (Script_unk2(0x879) != 0) {
        Script_SetRect(5, 6, 1, 1, 6, 6);
        Script_unk2_5(5, 6, 1, 1, 7, 6);
        Script_unk3_5(5, 6, 1, 1, 8, 6);
        Script_unk4_5(0, 1, 3, 1, 6, 5);
    }
    if (Script_unk3(0x815) != 0) {
        Script_Place(8, 0x780000, 0xe80000);
        Script_unk5_4(2, 10, 1, 1, 6, 14);
        Script_unk6_4(2, 10, 1, 1, 7, 14);
        Script_unk7_4(2, 10, 1, 1, 8, 14);
    }
}
