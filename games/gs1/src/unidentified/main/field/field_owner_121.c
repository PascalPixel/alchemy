#include "types.h"
#include "global_cells.h"

void Func_080c0a24(u32, u32, s32, s32, u32);

extern u16 Value_fffffe00;

void Func_080b86ec(void)
{
    void **slot = (void **)ADDR_03001E80;
    u8 *entity = slot[0];
    u8 *state = slot[32];
    volatile u32 *keys = (volatile u32 *)ADDR_03001AE8;

    if ((*keys & 512) != 0) {
        *(u16 *)(entity + 54) += 512;
    }
    if ((*keys & 256) != 0) {
        *(u16 *)(entity + 54) += (u16)(u32)&Value_fffffe00;
    }
    if (*(u32 *)(state + 20) == 0) {
        Func_080c0a24(0x780000, 0x780000, 0, 0, 0x10000);
    }
}
