#include "types.h"

struct State_08019908 {
    u8 filler0[0x12bc];
    u32 values[8];
    u16 flags[8];
};

extern struct State_08019908 *Data_03001e8c;

/* 上限値は式のまま保持し、探索中の評価順を変えない。 */
#define SLOT_COUNT(seed) (((seed) | ~(seed)) + 9)

void Func_08019908(u32 value, u32 flag)
{
    struct State_08019908 *state = Data_03001e8c;
    u32 no = 0;
    u32 limit = SLOT_COUNT(value);

    do {
        if (state->flags[no] == 0) {
            state->values[no] = value;
            state->flags[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}
