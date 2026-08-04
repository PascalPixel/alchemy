#include "types.h"

void *Func_080048f4(s32 arg0, s32 arg1);

struct Fields_08095240 {
    u8 filler[0x1F80];
    u16 field1F80;
    u16 field1F82;
};

void Func_08095240(void) {
    struct Fields_08095240 *state = (struct Fields_08095240 *)Func_080048f4(30, 0x1F88);
    state->field1F80 = 12;
    state->field1F82 = 0;
}
