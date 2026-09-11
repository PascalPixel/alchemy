#include "types.h"
#include "scene.h"

void Scene_RunScriptedStep953(void)
{
    Story_Run();
    Story_Apply(0x953, 1);
    Story_unk2_4();
}
