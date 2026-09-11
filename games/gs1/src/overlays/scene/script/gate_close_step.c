#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunGate(void)
{
    u32 i;
    s32 record;

    Script_unk6_3();
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Script_unk7_3();
    ((void (*)())Script_unk7)(0, 0);
    Script_unk8_2(4);
    Script_Run(-1, -1, -1, 0);
    Script_unk2_4(0x9999, 0x1333);
    Script_unk3_4(0x4c80000, -1, 0x880000, 1);
    record = Script_Check(0);
    if (record != 0) {
        Script_unk9_2(8, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_unk2(0);
    if (record != 0) {
        Script_unk10_2(5, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_unk3(0);
    if (record != 0) {
        Script_unk11_2(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Script_Place(8, 0x9999, 0x4ccc);
    Script_unk2_3(5, 0x9999, 0x4ccc);
    Script_unk3_3(1, 0x9999, 0x4ccc);
    Script_unk12_2(1, 2);
    Script_unk13_2(5, 2);
    Script_unk14_2(8, 2);
    Script_unk4_3(1, -16, 0);
    Script_unk15(5, 16, 0);
    Script_unk5_2(8, 0, -32);
    Script_unk16(1);
    Script_unk17(1, 0);
    Script_unk18(5, 0);
    Script_unk6_2(1, 0xc000, 0);
    Script_unk7_2(5, 0xc000, 0);
    Script_unk19(8);
    Script_unk20(8, 1);
    Script_unk21(40);
    Script_unk22(8, 2);
    Script_unk23(20);
    Script_unk8(8, 0x3000, 40);
    Script_unk9(8, 0x5000, 40);
    Script_unk10(8, 0x3000, 20);
    Script_unk24(8, 4, 20);
    Script_Do(0xfd3);
    Script_unk4_4(0x4008, 0);
    Script_unk25(20);
    Script_unk5_3(0x4c80000, -1, 0x940000, 1);
    Script_unk26(1, 2);
    record = Script_unk4(0);
    if (record != 0) {
        Script_unk27(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk28(5, 2);
    record = Script_unk5(0);
    if (record != 0) {
        Script_unk29(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk30(8, 2);
    record = Script_unk6(0);
    if (record != 0) {
        Script_unk31(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk32(1);
    Script_unk33(1, 0, 0);
    Script_unk34(5, 0, 0);
    Script_unk35(8);
    Script_unk36(8, 0, 0);
    Script_unk37(1, 1);
    Script_unk38(5, 1);
    Script_unk39(8, 1);
    Script_unk2_2(0x802);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Script_unk3_2(0x12f);
    Script_unk40();
}
