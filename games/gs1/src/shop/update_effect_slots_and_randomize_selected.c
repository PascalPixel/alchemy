#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

#define Shop_UpdateEffectSlotsAndRandomizeSelected Func_080b2ffc

void Func_0808a500(s32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Func_08009248(s32, u32);

void Shop_UpdateEffectSlotsAndRandomizeSelected(void)
{
    s32 p;
    s32 cnt;
    s32 work;
    s8 no;
    s32 offset;

    work = *(s32 *)ADDR_03001F2C;
    p = work + 0x3B0;
    cnt = 0x17;
    do {
        cnt -= 1;
        Func_0808a500(p);
        p += 0x48;
    } while (cnt >= 0);
    no = *(s8 *)(work + 0x3AB);
    if (no != -1) {
        Func_08009248(*(s32 *)(work + (offset = (no * 4) + 0x114)), (Rand() * 7) >> 16);
    }
}
