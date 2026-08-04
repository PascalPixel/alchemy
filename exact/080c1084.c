#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct State080c1084 {
    u8 padding_000[0x64e];
    u16 index;
    u16 field_650;
};

void Func_080c1084(void) {
    s32 _c0 = 0x03001E74;
    s8 *table;
    u16 index;
    s32 next;
    struct State080c1084 *state;

    state = *(struct State080c1084 **)_c0;
    if ((state != NULL) && (state->field_650 != 0)) {
        M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F90;
        M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x10;
        table = (s8 *)0x080C5C10;
        *(s16 *)0x04000054 = table[state->index];
        index = state->index;
        next = (index + 1) & 0xF;
        if ((u32) index > 0xEU) {
            next |= 0x10;
        }
        state->index = next;
    }
}
