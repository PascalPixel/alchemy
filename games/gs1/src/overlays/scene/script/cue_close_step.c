#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    Script_unk3_4();
    Script_unk4_4();
    Script_unk5_3();
    Script_Place(19, 0x3000, 0);
    Script_unk2_3(0, 0x9999, 0x4ccc);
    Script_unk3_3(0, 0x100, 0x294);
    Script_unk6_3(20);
    Script_Run(-1, -1, -1, 0);
    Script_Do(0x200);
    Script_unk7_3(188);
    Script_unk8_2(1);
    Script_unk9_2(2);
    Script_unk4_3(19, 0x1000000, 0x2780000);
    Script_unk10_2(1);
    Script_unk5_2(19, 0x9999, 0x4ccc);
    Script_unk6_2(19, 0x100, 0x284);
    Script_unk11_2(1);
    Script_unk12_2(2);
    Script_unk13_2(20);
    Script_unk14_2(19, 2);
    Script_unk2_2(0x145e);
    Script_unk15(19, 0, 10);
    Script_unk7_2(0, 0x100, 40);
    Script_unk8(0, 0x108, 0x294);
    Script_unk9(0, 0x8000, 0);
    Script_unk10(19, 248, 0x294);
    Script_unk11(19, 0x1000, 40);
    Script_unk16(19, 4);
    Script_unk17(19, 0);
    Script_unk18(19, 3);
    Script_Check(19, 0);
    Script_unk19(19, 2);
    Script_unk20(19, 0, 10);
    Script_unk12(0, 0x101, 60);
    Script_unk2_4(19, 0x102);
    Script_unk21(60);
    Script_unk22(19, 1);
    Script_unk23(19, 0, 10);
    Script_unk24(19, 3);
    Script_unk25(19, 0);
    Script_unk13(19, 0xcccc, 0x6666);
    Script_unk14(19, 248, 0x304);
    Script_unk26(19, 0, 0);
    Script_unk3_2(0x12f);
    Script_unk4_2(0x84f);
    Script_unk27();
}
