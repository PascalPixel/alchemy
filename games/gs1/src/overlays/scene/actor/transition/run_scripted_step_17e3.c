#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_scripted_step_17e3.h"

void Scene_RunScriptedStep17E3(void)
{
    Actor_Run();
    Actor_Apply(0x17E3, 1);
    Actor_Run2();
}
