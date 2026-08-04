#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08091200(s32 arg0, s32 arg1);
void Func_08091254(s32 value);

void Func_0809748c(void) {
    s32 value;
    u8 *state;

    state = *(u8 **)0x03001EBC;
    value = (s32)(state + 0x236);
    Func_08091200(value, 2);
    if (M2C_FIELD(state, s16 *, 0xCB8) != 0) {
        Func_08091200(0x10001, 1);
    } else {
        Func_08091200(value, 1);
    }
    Func_08091254(8);
}
