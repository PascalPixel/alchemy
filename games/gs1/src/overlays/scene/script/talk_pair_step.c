#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunTalk(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Place(0, 0x28000, 0x14000);
    Script_unk2_3(0, a0, a1);
    Script_unk2_4(0, 4, 0);
    Script_unk3_4(0, 7);
    Script_unk4_4(0);
    Script_unk5_3(0, 6);
    Script_unk6_3();
}
