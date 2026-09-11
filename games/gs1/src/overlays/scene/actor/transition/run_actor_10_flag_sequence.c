#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_10_flag_sequence.h"

void Scene_RunActor10Flag315Sequence(void)
{
    Actor_Do(241);
    Actor_Place(10, -160, 0);
    Actor_Do2(0x121);
    Actor_Do3(0x315);
    Actor_Do4(2);
    Actor_Run();
}
