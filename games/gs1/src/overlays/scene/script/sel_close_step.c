#include "types.h"
#include "scene.h"

void Scene_RunSel(void)
{
    s32 rec7;
    s32 record;
    s32 record2;
    s16 v10;
    s16 v18;

    if (Script_Check(0x256) == 0) {
        record = Script_unk2(0);
        v10 = *(s16 *)(record + 10);
        record2 = Script_unk3(0);
        v18 = *(s16 *)(record2 + 18);
        if ((u32)(v10 - 84) <= 7) {
            if (v18 > 211) {
                if (v18 <= 219) {
                    Script_unk6(record2);
                    Script_Do(0x256);
                    Script_Run(5);
                    record = Script_unk7(0);
                    *(volatile s32 *)(record + 12) += -0x20000;
                    rec7 = Script_unk4(0);
                    record = Script_unk5(0);
                    *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
                    Script_SetRect(5, 2, 5, 11, 1, 1);
                    Script_unk2_4(217);
                    Script_Place(0x200e010, 9, 7);
                    Script_unk3_4();
                }
            }
        }
    }
}
