#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Do(0x200d160);
    Script_unk2_4(1);
    Script_Place(0, 0xe80000, 0x27c0000);
    Script_unk3_4(0, 15);
    record = Script_Check(0);
    Script_unk4_4(record, 0);
    Script_unk5_3(1);
    Script_unk6_3(0, 0);
    Script_unk7_3();
    Script_unk8_2(1);
    Script_unk9_2(22);
    Script_unk10_2(21);
    Script_unk11_2(1);
    Script_unk12_2(22, 0, 0);
    Script_unk13_2(21, 0, 0);
    Script_unk14_2(20, 0, 0);
    record = Script_unk2(20);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk2_3(23, 0xe80000, 0x28a0000);
    record = Script_unk3(23);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk15(1);
    Script_unk16(20, 23);
}
