#include "types.h"
#include "scene.h"

extern u8 gWork[];

void Scene_RunScene3bf(void)
{
    u32 i;
    s32 record;

    Map_Run(11, 1);
    Map_unk2_4();
    Map_unk3_4(60);
    Map_Do(0x247c);
    Map_unk4_4(13, 0);
    Map_Place(11, 0x10000, 0x8000);
    Map_unk2_3(15, 0x10000, 0x8000);
    Map_unk3_3(11, 0x1d8, 180);
    Map_unk4_3(15, 0x1d8, 180);
    Map_unk5_4(11, 1);
    Map_unk6_4(11);
    Map_unk7_4(11, 4);
    Map_unk8_4(30);
}
