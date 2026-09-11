#include "types.h"
#include "scene.h"

extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

void Scene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_unk6_4();
    } else {
        State_unk7_4();
        rec2 = State_Check(a0, 3);
        if (rec2 != 0) {
        } else {
            State_Do(0x20a6);
            State_Run(0x30000, 0x6000);
            State_unk2_4(0x2f00000, -1, 0xc00000, 1);
            State_unk8_4();
            State_unk9_4(60);
            State_unk3_4(0x10000, 0x2000);
            State_unk4_4(0x2f00000, -1, 0xe00000, 1);
            State_unk10_4();
            State_unk11_4(a0, 0);
            State_unk12_4(0, 0x358, 0x108);
            State_unk13_4(10);
            State_Place(0, 0x18000, 0xc000);
            State_unk2_3(0, 0x358, 0x108);
            State_unk3_3(0, 0x358, 232);
            State_unk14_4(a0, 0);
            State_unk4_3(0, 0x348, 232);
            State_unk15_4(10);
            State_unk5_3(33, -64, 0);
            State_unk5_4(0x2f00000, -1, 0xd80000, 1);
            State_unk16_4(0, 1);
            State_unk17_4(10);
            State_unk6_3(0, 0x10000, 0x8000);
            State_unk7_3(0, 0x2f8, 232);
            State_unk18_4(10);
            State_unk8_3(0, 0x4000, 30);
            State_unk19_4(a0, 0);
            State_unk20_4(0);
            State_unk21_4(0, 0);
            State_unk9_3(33, 0x3480000, 0xe80000);
            State_unk22_4(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            State_unk2_2(0x20a5);
            State_unk23_4(a0, 0);
        }
        L_020016b0:;
        State_unk10_3(rec2, a0, 3);
        State_unk24_4();
    }
}
