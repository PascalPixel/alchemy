#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

void Func_0808a500(s32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Func_08009248(s32, u32);

void Func_080b2ffc(void)
{
    s32 var_r5;
    s32 var_r6;
    s32 temp_r7;
    s8 temp_r5;
    s32 offset;

    temp_r7 = *(s32 *)ADDR_03001F2C;
    var_r5 = temp_r7 + 0x3B0;
    var_r6 = 0x17;
    do {
        var_r6 -= 1;
        Func_0808a500(var_r5);
        var_r5 += 0x48;
    } while (var_r6 >= 0);
    temp_r5 = *(s8 *)(temp_r7 + 0x3AB);
    if (temp_r5 != -1) {
        Func_08009248(*(s32 *)(temp_r7 + (offset = (temp_r5 * 4) + 0x114)), (Rand() * 7) >> 16);
    }
}
