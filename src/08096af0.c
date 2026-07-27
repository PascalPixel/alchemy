#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080984c0(void);
void Func_08099738(void);
void Func_0809b648(void);

void Func_08096af0(void) {
    u32 temp_r3;

    temp_r3 = (s16) M2C_FIELD(*(void **)0x03001F30, s16 *, 0x1E);
    switch (temp_r3) {
    case 8:
        Func_080984c0();
        return;
    case 10:
        Func_08099738();
        return;
    case 16:
        Func_0809b648();
        return;
    }
}
