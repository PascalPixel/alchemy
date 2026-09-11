#include "scene.h"
#include "abi/overlays/scene/story/actor_sequence/set_three_cells_6_11_flag_241.h"
typedef signed int s32;

void State_SetThreeCellsAt6x11AndFlag241(void)
{
    Story_SetRect(6, 11, 1, 1, 7, 11);
    Story_SetRect2(6, 11, 1, 1, 8, 11);
    Story_SetRect3(6, 11, 1, 1, 9, 11);
    Story_Do(0x241);
}
