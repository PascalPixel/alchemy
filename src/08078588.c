#include "types.h"
#include "item.h"
#include "owner_state.h"

/* 所持品追加。積み重ね可能な品は同一番号の枠を探して個数を増やし、
   そうでなければ空き枠へ入れる。戻り値は枠番号、失敗は -1。 */
s32 Func_08078588(s32 owner, s32 item)
{
    struct OwnerInventoryState *state = Func_08077394(owner);
    struct ItemDefinition *def = Func_08078414(item);
    s32 index;

    if ((def->flags & 0x10) != 0) {
        index = 0;
        if (((state->inventory[index] ^ item) & 0x1ff) != 0) {
            do {
                index++;
                if (index > 14)
                    break;
            } while (((state->inventory[index] ^ item) & 0x1ff) != 0);
        }
        if (index != 15) {
            s32 entry = state->inventory[index];
            u32 count = ((u32)entry >> 11) + 1;

            if (count > 29)
                return -1;
            {
                s32 value = 0x7ff;

                value &= entry;
                value |= count << 11;
                state->inventory[index] = value;
            }
            return index;
        }
    }

    index = 0;
    do {
        if (state->inventory[index] == 0) {
            state->inventory[index] = item;
            return index;
        }
        index++;
    } while (index <= 14);
    return -1;
}
