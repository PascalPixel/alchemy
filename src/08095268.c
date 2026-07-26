#include "types.h"

u8 *Func_080048f4(s32, s32);

struct Fields_08095268 {
    u8 filler[0x1F80];
    u16 field1F80;
    u16 field1F82;
};

void Func_08095268(void) {
    struct Fields_08095268 *state = (struct Fields_08095268 *)Func_080048f4(30, 0x1F88);
    state->field1F80 = 128;
    state->field1F82 = 1;
}
