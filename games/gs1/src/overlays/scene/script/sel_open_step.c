#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunSel(s32 a0)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_unk2_4(228);
    Script_Place(0, 0x6666, 0x3333);
    Script_unk3_4(0, 2);
    Script_unk2_3(0, 0, -8);
    record = Script_Check(0);
    Script_unk4_4(record, 0);
    Script_unk5_3(8);
    Script_unk6_3(0, ((a0 << 19) + 0x80000), 0);
    Script_unk7_3(30);
}
