#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_11_offset_sequence.h"

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Actor_Run.
 */

void Scene_RunActor11Offset128Sequence(void)
{
    Actor_Do(241);
    Actor_Place(11, 0, 128);
    Actor_Run2();
    Actor_Run(0x121);
    Actor_Do2(2);
    Actor_Run3();
}
