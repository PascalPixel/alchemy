#include "types.h"
#include "scene.h"

extern u8 gWork[];

u8 *Script_Run();

void Scene_RunSel(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Script_unk2_4();
    Script_Check(0x200ba65, 0xc80);
    Script_Place(0, 0x28000, 0x14000);
    Script_unk3_4(0, 1);
    *(u8 *)(Script_unk2(0) + 90) &= 254;
    Script_unk4_4(228);
    if (*(s16 *)((s32)p5 + 0x16c) == 2) {
        Script_unk2_3(0, 232, (154 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 3) {
        Script_unk2_3(0, (180 << 1), (182 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 4) {
        Script_unk2_3(0, 248, (198 << 2));
    } else {
        Script_unk3_3(0, 696, 592);
        Script_unk4_3(0, 696, 600);
        Script_Do(30);
    }
    Script_unk5_3(0);
    {
        u8 *record = Script_Run(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Script_unk2_2(0x200ba65);
    Script_unk6_3();
}
