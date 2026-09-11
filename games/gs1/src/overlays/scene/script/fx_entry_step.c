#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Script_unk3_4();
    Script_Run(-1, -1, -1, 0);
    Script_unk4_4(1);
    Script_unk5_3(20, 0, 0);
    Script_unk6_3(22, 0, 0);
    Script_unk7_3(24, 0, 0);
    Script_unk8_2(25, 0, 0);
    Script_unk9_2(26, 0, 0);
    Script_unk10_2(27, 0, 0);
    Script_unk11_2(0, 0, 0);
    Script_unk12_2(23, 0, 0);
    record = Script_Check(23);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Place(21, 0xe80000, 0x28a0000);
    record = Script_unk2(21);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk2_4(0xe80000, -1, 0x27c0000, 0);
    Script_unk13_2();
    Script_unk14_2(1);
    Script_unk15(23, 21);
}
