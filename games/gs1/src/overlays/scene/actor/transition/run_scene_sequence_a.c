#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_scene_sequence_a.h"

extern u8 gWork[];

void Scene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Actor_Run();
    record = Actor_Check(8);
    Actor_Run2(record, 0);
    Actor_Run3();
    Actor_Place(0, 0x20000, 0x1999);
    Actor_Place2(0, 0x108, 196);
    Actor_Run4();
}
