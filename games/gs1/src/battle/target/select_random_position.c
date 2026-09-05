#include "types.h"
#include "battle_command.h"
#include "battle_runtime.h"
#include "fixed_math.h"

struct SlotArray { s16 items[64]; };


s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{
    u16 positions[6];
    struct SlotArray *order;
    s16 *entry;
    u16 *cursor;
    s32 value;
    s32 count;
    s32 index;
    s32 slot;
    s32 tail;
    s32 offset;

    count = 0;
    order = (struct SlotArray *)BATTLE_TURN_ORDER;

    if (require_living_unit != 0) {
        for (;;) {
            index = 0;
            slot = 44;
            if (order->items[slot] != 255) {
                entry = order->items;
                do {
                    value = entry[slot];
                    if (value != 254) {
                        if (BattleUnit_Get(value)->hp != 0) {
                            positions[count] = index | 0x100;
                            count++;
                        }
                    }
                    slot++;
                    index++;
                } while (entry[slot] != 255);
            }
            goto pick;
        }
    } else {
        index = 0;
        slot = 50;
        tail = 50;
        offset = tail * 2;
        entry = (s16 *)(order->items + 1);
        if (*(s16 *)((char *)entry + offset) != 255) {
            cursor = (u16 *)entry;
            do {
                if ((s16)cursor[slot] != 254) {
                    positions[count] = index | 0x180;
                    count++;
                }
                slot++;
                tail++;
                index++;
            } while (entry[tail] != 255);
        }
    }

pick:
    if (count == 0)
        return 0;
    return positions[(u32)(Random16() * count) >> 16];
}
