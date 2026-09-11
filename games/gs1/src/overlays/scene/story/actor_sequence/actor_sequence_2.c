#include "scene.h"

/* overlays/scene/story/actor_sequence/no_op.c */
void Scene_NoOp(void) { }

/* overlays/scene/story/actor_sequence/set_three_cells_6_11_flag_241.c */
typedef signed int s32;

void State_SetThreeCellsAt6x11AndFlag241(void)
{
    Story_SetRect(6, 11, 1, 1, 7, 11);
    Story_unk2_5(6, 11, 1, 1, 8, 11);
    Story_unk3_5(6, 11, 1, 1, 9, 11);
    Story_Do(0x241);
}
