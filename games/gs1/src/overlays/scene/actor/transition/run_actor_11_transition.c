#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_11_transition.h"

void Scene_RunActor11Transition(void)
{
    Actor_Run(241);
    Actor_Place(11, 0, 48);
    Actor_Do(0x121);
    Actor_Run2();
    Actor_Do2(2);
    Actor_Run3();
}
