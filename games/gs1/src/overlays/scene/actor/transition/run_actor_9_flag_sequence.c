#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_9_flag_sequence.h"

void Scene_RunActor9Flag314Sequence(void)
{
    Actor_Do(0xF1);
    Actor_Place(9, -128, 0);
    Actor_Do2(0x121);
    Actor_Do3(0x314);
    Actor_Do4(2);
    Actor_Run();
}
