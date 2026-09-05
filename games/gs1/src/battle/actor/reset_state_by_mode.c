#include "types.h"

#define BattleUnit_ResetStateByMode Func_080b2da8

u8 *Runtime_GetObject(s32);
void Func_08077128(s32);
u8 *Item_GetData(u16);
void BattleUnit_Recalculate(s32);

void BattleUnit_ResetStateByMode(s32 id, s32 mode) {
    u8 *state;

    state = Runtime_GetObject(id);
    if (mode == 0) {
        *(u16 *)(state + 56) = *(u16 *)(state + 52);
        Func_08077128(id);
    } else if (mode == 1) {
        state[0x131] = 0;
    } else if (mode == 2) {
        state[320] = 0;
    } else if (mode == 3) {
        volatile u16 *entry;
        s32 i;
        s32 mask;

        mask = 0x200;
        entry = (volatile u16 *)state;
        i = 14;
        entry = (volatile u16 *)((u8 *)entry + 216);
        do {
            if ((*entry & mask) != 0 &&
                (Item_GetData(*entry)[3] & 1) != 0) {
                *entry ^= mask;
                BattleUnit_Recalculate(id);
            }
            i--;
            entry++;
        } while (i >= 0);
    }
}
