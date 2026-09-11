#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/talk_pair_step.h"

extern u8 gWork[];

void Scene_RunTalk(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Place(0, 0x28000, 0x14000);
    Script_Place2(0, a0, a1);
    Script_Run2(0, 4, 0);
    Script_Run3(0, 7);
    Script_Run4(0);
    Script_Run5(0, 6);
    Script_Run6();
}
