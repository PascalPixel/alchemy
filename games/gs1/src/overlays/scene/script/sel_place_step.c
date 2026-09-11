#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/sel_place_step.h"

extern u8 gWork[];

void Scene_RunSel(void)
{
    u32 i;
    s32 record;

    if (Script_Check(0x310) != 0) {
        record = Script_Check2(0x311);
        if (record == 0) {
            goto L_02001140;
        }
        record = Script_Check3(0x312);
        if (record == 0) {
            goto L_02001140;
        }
        Script_Do(0x876);
        Script_Run(30);
        Script_Place(0x10000, 0x10000, 0x10000);
        Script_Run2(141);
        Script_Run3(60);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
        Script_Run4();
        Script_Run5();
        Script_Do2(0x121);
        Script_Place2(-1, -1, 0xe666);
        Script_Run6();
        Script_Run7(13);
    } else {
        L_02001140:;
        Script_Do3(0x876);
    }
}
