#include "types.h"

extern u8 *Data_03001f2c;
void Func_08009020(void *, s32);
s32 Func_08004278(s32);

void Func_080a3c98(void) {
    u8 *state;
    s8 i;

    state = Data_03001f2c;
    if (state[0x219] != 0) {
        i = 0;
        do {
            s32 objectOffset = i * 4 + 276;
            void *object = *(void **)(state + objectOffset);
            Func_08009020(object, 1);
            i++;
        } while (i < state[0x219]);
    }
    Func_08004278(0x080a3c09);
}
