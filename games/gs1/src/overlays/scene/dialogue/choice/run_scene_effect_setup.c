#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/choice/run_scene_effect_setup.h"

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Talk_Place(actor, axis, offset);
}

void RunSceneEffectSetup(void)
{
    Talk_Run();
    Talk_Run2(0, 32768, 16384);
    Talk_Run3(158);
    Talk_Run4(33604944, 36, 10);
    SetOffset(0, 2, -16);
    Talk_Run5(16);
    Talk_Run6(2);
    Talk_Run7();
}
