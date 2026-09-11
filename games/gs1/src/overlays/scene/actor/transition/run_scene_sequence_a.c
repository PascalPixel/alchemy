#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Actor_Run();
    record = Actor_Check(8);
    Actor_unk2_4(record, 0);
    Actor_unk3_4();
    Actor_Place(0, 0x20000, 0x1999);
    Actor_unk2_3(0, 0x108, 196);
    Actor_unk4_4();
}
