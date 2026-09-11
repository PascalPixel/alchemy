#include "types.h"
#include "scene.h"

/* overlays/scene/actor/transition/run_steps_to.c */
void Scene_RunSteps30FTo312(void)
{
    Actor_Do(0x30F);
    Actor_unk2_2(0x310);
    Actor_unk3_2(0x311);
    Actor_unk4_2(0x312);
}

/* overlays/scene/actor/transition/run_actor_11_offset_sequence.c */
/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Actor_Run.
 */

void Scene_RunActor11Offset128Sequence(void)
{
    Actor_Do(241);
    Actor_Place(11, 0, 128);
    Actor_unk2_4();
    Actor_Run(0x121);
    Actor_unk2_2(2);
    Actor_unk3_4();
}

/* overlays/scene/actor/transition/run_actor_11_flags_312_sequence.c */
void Scene_RunActor11Flags30fTo312Sequence(void)
{
    Actor_Do(0xF1);
    Actor_Place(0xB, 0, -16);
    Actor_unk2_2(0x121);
    Actor_unk3_2(0x30F);
    Actor_unk4_2(0x310);
    Actor_unk5_2(0x311);
    Actor_unk6_2(0x312);
    Actor_unk7_2(2);
    Actor_Run();
}
