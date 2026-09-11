#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/sel_cam_step.h"

extern u8 gWork[];

u8 *Script_Run();

void Scene_RunSel(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Script_Run2();
    Script_Check(0x200ba65, 0xc80);
    Script_Place(0, 0x28000, 0x14000);
    Script_Run3(0, 1);
    *(u8 *)(Script_Check2(0) + 90) &= 254;
    Script_Run4(228);
    if (*(s16 *)((s32)p5 + 0x16c) == 2) {
        Script_Place2(0, 232, (154 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 3) {
        Script_Place2(0, (180 << 1), (182 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 4) {
        Script_Place2(0, 248, (198 << 2));
    } else {
        Script_Place3(0, 696, 592);
        Script_Place4(0, 696, 600);
        Script_Do(30);
    }
    Script_Run5(0);
    {
        u8 *record = Script_Run(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Script_Do2(0x200ba65);
    Script_Run6();
}
