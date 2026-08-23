#include "types.h"
#include "global_cells.h"

struct State080a8034 {
    u8 padding_00[0x20];
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2c;
    u8 padding_30[0xe0];
    s8 field_110;
    s8 field_111;
    s8 field_112;
    s8 field_113;
};

s32 Func_080a1814(void *);
s32 InitializeEntryObjects(s32, s32, s32, s32, s32);

void Func_080a8034(void) {
    struct State080a8034 *state;

    state = *(struct State080a8034 **)ADDR_03001F2C;
    InitializeEntryObjects(Func_080a1814(state), 2, 2, 8, 0);
    state->field_28 = 0;
    state->field_24 = 0;
    state->field_2c = 0;
    state->field_20 = 0;
    state->field_110 = 0;
    state->field_111 = 0;
    state->field_112 = 8;
    state->field_113 = 2;
}
