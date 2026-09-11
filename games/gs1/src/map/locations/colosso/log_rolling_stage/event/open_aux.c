#include "types.h"
#include "scene.h"

extern u8 gCell[];
extern u8 gOv[];
extern u8 gWork[];
extern s16 gCell2[][1];

void Scene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (gCell2[225][0] == 2) {
        Map_unk5_4();
    } else {
        Map_unk6_4();
        rec2 = Map_Check(a0, 5);
        if (rec2 != 0) {
        } else {
            Map_Do(0x20c3);
            Map_Run(0x30000, 0x6000);
            Map_unk2_4(0x4380000, -1, 0xa80000, 1);
            Map_unk7_4();
            Map_unk8_4(30);
            Map_unk9_4(a0, 0);
            Map_unk10_4(a0, 0);
            Map_Place(0, 0x3d8, 184);
            Map_unk2_3(0, 0x18000, 0xc000);
            Map_unk11_4(0, 0x3e0, 184);
            Map_unk3_3(0, 0x4ccc, 0x2666);
            Map_unk4_3(0, 0x460, 184);
            Map_unk12_4(120);
            Map_unk3_4(0, 0x101);
            Map_unk13_4(120);
            Map_unk14_4(0);
            Map_unk15_4(0, 1);
            Map_unk4_4(0, 0x100);
            Map_unk5_3(0, 0x105, 0);
            rec7 = Map_unk2(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Map_unk16_4(1);
            }
            Map_unk6_3(0, 0x103, 60);
            Map_unk7_3(0, 0x460, 184);
            Map_unk17_4(a0, 0);
            Map_unk18_4(0);
            {
                u8 *flag = (u8 *)gCell2;

                flag[498] = 1;
            }
            Map_unk19_4(0);
            Map_unk20_4(0, 0);
            Map_unk21_4(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Map_unk2_2(0x20c2);
            Map_unk22_4(a0, 0);
        }
        L_02002494:;
        Map_unk8_3(rec2, a0, 5);
        Map_unk23_4();
    }
}
