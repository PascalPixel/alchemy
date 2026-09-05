#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0809abb4();

void BattleEffect_MarkChildAndRunFallbackTransition(void) {
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(*(void **)ADDR_03001F30, void **, 0x14), s8 *, 0x5B) = 1;
    Func_0809abb4();
}
