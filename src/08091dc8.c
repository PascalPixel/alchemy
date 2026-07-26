#include "types.h"

struct Fields_08091dc8 {
    u8 filler[0x1C0];
    s32 field1C0;
    u16 field1C4;
    u16 field1C6;
    s32 field1C8;
};

extern struct Fields_08091dc8 *Data_03001ebc;
void Func_0808fefc(s32, s32);

void Func_08091dc8(void) {
    struct Fields_08091dc8 *state = Data_03001ebc;
    Func_0808fefc(state->field1C0, state->field1C8);
    state->field1C6 = 1;
}
