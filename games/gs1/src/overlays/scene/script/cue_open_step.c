#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    if (Script_Check(0x84e) != 0) {
        Script_Run();
        Script_unk2_4(0, 19, 0);
        Script_Place(19, 0x9999, 0x4ccc);
        Script_unk2_3(19, 0x26e, 0x2fc);
        Script_unk3_3(19, 0xf000, 20);
        Script_unk3_4(19, 3);
        Script_unk4_4(17, 3);
        Script_unk5_3(20);
        Script_unk6_3(19, 0, 0);
        Script_unk7_3(20);
        Script_unk8_2(19, 3);
        Script_Do(0x1749);
        Script_unk9_2(19, 0, 10);
        Script_unk4_3(19, 0xcccc, 0x6666);
        Script_unk5_2(19, 0x23a, 0x2f6);
        Script_unk10_2(19, 0, 0);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
        Script_unk2_2(0x85e);
        Script_unk3_2(0x333);
        Script_unk11_2();
    }
}
