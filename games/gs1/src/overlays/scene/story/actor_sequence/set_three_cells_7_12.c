#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/actor_sequence/set_three_cells_7_12.h"

/* Three adjacent collision cells used by the scene's lower doorway. */

void State_SetThreeCellsAt7x12(void)
{
    Story_SetRect(7, 12, 1, 1, 7, 11);
    Story_SetRect2(7, 12, 1, 1, 8, 11);
    Story_SetRect3(7, 12, 1, 1, 9, 11);
}
