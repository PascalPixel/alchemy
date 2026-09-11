#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/scenario_dispatch/run_scripted_step_953.h"

void Scene_RunScriptedStep953(void)
{
    Story_Run();
    Story_Apply(0x953, 1);
    Story_Run2();
}
