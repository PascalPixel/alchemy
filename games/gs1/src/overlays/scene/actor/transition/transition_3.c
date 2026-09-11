#include "types.h"
#include "scene.h"

/* overlays/scene/actor/transition/run_actor_8_transition.c */
void Scene_RunActor8Transition313(void)
{
    Actor_Do(0xF1);
    Actor_Place(8, 0, -112);
    Actor_unk2_2(0x121);
    Actor_unk3_2(0x313);
    Actor_unk4_2(2);
    Actor_Run();
}

/* overlays/scene/actor/transition/run_actor_9_flag_sequence.c */
void Scene_RunActor9Flag314Sequence(void)
{
    Actor_Do(0xF1);
    Actor_Place(9, -128, 0);
    Actor_unk2_2(0x121);
    Actor_unk3_2(0x314);
    Actor_unk4_2(2);
    Actor_Run();
}
