#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/transition/run_actor_11_flags_312_sequence.h"

void Scene_RunActor11Flags30fTo312Sequence(void)
{
    Actor_Do(0xF1);
    Actor_Place(0xB, 0, -16);
    Actor_Do2(0x121);
    Actor_Do3(0x30F);
    Actor_Do4(0x310);
    Actor_Do5(0x311);
    Actor_Do6(0x312);
    Actor_Do7(2);
    Actor_Run();
}
