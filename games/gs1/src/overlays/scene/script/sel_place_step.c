#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunSel(void)
{
    u32 i;
    s32 record;

    if (Script_Check(0x310) != 0) {
        record = Script_unk2(0x311);
        if (record == 0) {
            goto L_02001140;
        }
        record = Script_unk3(0x312);
        if (record == 0) {
            goto L_02001140;
        }
        Script_Do(0x876);
        Script_Run(30);
        Script_Place(0x10000, 0x10000, 0x10000);
        Script_unk2_4(141);
        Script_unk3_4(60);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
        Script_unk4_4();
        Script_unk5_3();
        Script_unk2_2(0x121);
        Script_unk2_3(-1, -1, 0xe666);
        Script_unk6_3();
        Script_unk7_3(13);
    } else {
        L_02001140:;
        Script_unk3_2(0x876);
    }
}
