#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "battle_party.h"
#include "battle_escape.h"

/* battle/actor/clear_field_12b_for_group.c */
u8 *FunctionHead_08077008(s32);
void BattleUnit_Recalculate(u16 id);

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = Actor_Apply(3, ids);
    for (index = 0; index < count; index++) {
        FunctionHead_08077008(ids[index])[0x12b] = 0;
        BattleUnit_Recalculate(ids[index]);
    }
}
