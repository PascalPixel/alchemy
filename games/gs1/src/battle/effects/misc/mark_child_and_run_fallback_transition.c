#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/mark_child_and_run_fallback_transition.h"
#include "global_cells.h"


void BattleFx_MarkChildAndRunFallbackTransition(void)
{
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(*(void **)ADDR_03001F30, void **, 0x14), s8 *, 0x5B) = 1;
    Battle_Check();
}
