#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/talk_flag_step.h"

extern u8 gOv[];

void Scene_RunTalk(void)
{
    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Script_Check(22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(volatile s32 *)((s32)rec + 80);
        p6[38] = record;
        *(u8 *)(((s32)p6 + 38) + 1) = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Script_Check2(17, 0x608);
        Script_Run(230);
        Script_Place(p6[28], 128, (rec7 + 0x400));
        Script_Run2(17);
        *(volatile s32 *)gOv = (s32)rec;
    }
}
