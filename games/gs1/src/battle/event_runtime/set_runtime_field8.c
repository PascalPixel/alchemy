#include "types.h"
#include "scene.h"
#include "global_cells.h"


void BattleEv_SetRuntimeField8(void)
{
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}
