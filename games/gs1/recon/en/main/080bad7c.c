#include "types.h"
#include "battle_command.h"
#include "battle_runtime.h"
#include "fixed_math.h"

#define BattleTarget_SelectRandomPosition Func_080bad7c

s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{
    u16 positions[6];
    struct BattleTurnOrder *turn_order;
    s16 *entry;
    s32 count;
    s32 index;
    s32 unit_id;

    turn_order = BATTLE_TURN_ORDER;
    count = 0;

    if (require_living_unit != 0) {
        index = 0;
        if (turn_order->normal[0] != 255) {
            entry = turn_order->normal;
            do {
                unit_id = *entry;
                if (unit_id != 254) {
                    if (BattleUnit_Get(unit_id)->hp != 0) {
                        positions[count] = index | 0x100;
                        count++;
                    }
                }
                entry++;
                index++;
            } while (*entry != 255);
        }
    } else {
        index = 0;
        if (turn_order->mirrored[0] != 255) {
            entry = turn_order->mirrored;
            do {
                unit_id = *entry;
                if (unit_id != 254) {
                    positions[count] = index | 0x180;
                    count++;
                }
                entry++;
                index++;
            } while (*entry != 255);
        }
    }

    if (count == 0)
        return 0;
    return positions[(u32)(count * Random16()) >> 16];
}
