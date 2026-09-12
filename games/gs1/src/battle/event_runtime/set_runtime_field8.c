#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_event.h"
#include "../../../include/types.h"
#include "../../../include/battle_actor.h"
#include "../../../include/battle_calc.h"
#include "../../../include/battle_command.h"
#include "../../../include/battle_efx.h"
#include "../../../include/battle_event.h"
#include "../../../include/battle_msg.h"
#include "../../../include/battle_runtime.h"
#include "../../../include/battle_summon.h"
#include "../../../include/battle_types.h"
#include "../../../include/battle_work.h"
#include "../../../include/runtime_1e74.h"
#include "../../../include/runtime_mem.h"

/* battle/ev_set_runtime_field8.c */
/* battle/event_runtime/set_runtime_field8.c */
void BattleEv_SetRuntimeField8(void)
{
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}
