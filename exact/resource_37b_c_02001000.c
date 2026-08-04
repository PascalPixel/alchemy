#include "types.h"
#define NULL ((void *)0)

u16 *Func_02003472(s32);
void Func_02003548(void);

void Func_02001000(void) {
    u16 *p = Func_02003472(0);
    if (p[3] == 0x4000) {
        Func_02003548();
    }
}
