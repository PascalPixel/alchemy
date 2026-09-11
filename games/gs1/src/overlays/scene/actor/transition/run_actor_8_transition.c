#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_8_transition.h"

void Scene_RunActor8Transition313(void)
{
    Actor_Do(0xF1);
    Actor_Place(8, 0, -112);
    Actor_Do2(0x121);
    Actor_Do3(0x313);
    Actor_Do4(2);
    Actor_Run();
}
