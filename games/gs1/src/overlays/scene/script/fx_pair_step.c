#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunScript(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 record;

    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x100;
    Script_unk2_4();
    Script_unk3_4();
    Script_unk4_4(20);
    Script_unk5_3();
    Script_Place(a0, 0xd80000, 0x24c0000);
    Script_unk2_3(a0, 0xcccc, 0x6666);
    Script_unk3_3(a0, 216, 0x258);
    Script_unk4_3(a0, 218, 0x25c);
    Script_unk5_2(a0, 234, 0x25c);
    Script_unk6_2(a0, 236, 0x26a);
    Script_unk7_2(a0, 0x5000, 20);
    Script_unk6_3(a0, 3);
    Script_unk7_3(20);
    Script_Run(a1, 0x5000);
    Script_unk8_2(a1, 4, 40);
    Script_unk9_2(a1, 2);
    Script_Do(0x1e39);
    Script_unk10_2(a1, 0, 20);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Script_unk11_2();
    Script_unk12_2();
    Script_unk13_2(10);
    p10 = a0;
}
