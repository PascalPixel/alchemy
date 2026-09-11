#include "types.h"
#include "scene.h"

/* Three adjacent collision cells used by the scene's lower doorway. */

void State_SetThreeCellsAt7x12(void)
{
    Story_SetRect(7, 12, 1, 1, 7, 11);
    Story_unk2_5(7, 12, 1, 1, 8, 11);
    Story_unk3_5(7, 12, 1, 1, 9, 11);
}
