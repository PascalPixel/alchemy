#include "types.h"

u32 Func_08004458(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Func_08004458
void Func_080030f8(u32);
s32 Func_080b6b40(s32 arg0, u16 *arg1);
void Func_080b8064(s32);
void Func_080151c8(s32);

s32 Func_080b8824(s16 *unused)
{
    s16 values[14];
    s32 count;
    s32 shown;
    s32 index;

    (void)unused;
    if (((u32)(Rand() << 4) >> 16) != 0) {
        count = Func_080b6b40(1, values);
        shown = 0;
        if (count != 0) {
            index = 0;
            do {
                Func_080b8064(values[index]);
                shown++;
                Func_080030f8(8);
                index++;
            } while (shown != count);
        }
        Func_080030f8(22);
        return 1;
    }
    Func_080151c8(0x844);
    return 0;
}
