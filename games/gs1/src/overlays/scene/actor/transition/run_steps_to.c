#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_steps_to.h"

void Scene_RunSteps30FTo312(void)
{
    Actor_Do(0x30F);
    Actor_Do2(0x310);
    Actor_Do3(0x311);
    Actor_Do4(0x312);
}
