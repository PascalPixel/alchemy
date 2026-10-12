/* Near miss: differ 2 of 97 (one reordered row). After the DMA the native loads the halfword mask 0x1f from the pool (ldr r7) before i = 0 and the word 31 are built; the draft builds those two first. All three have priority 2 in sched2. The twin in The Broken Seal needs no device. */
#include "resource.h"
#include "dma.h"

void Palette_StepTowardResource(s32 resource_id)
{
    u16 target[64];
    u16 *color = (u16 *)0x05000000;
    s32 i;

    Dma_Set(Resource_GetTableEntry(resource_id), target, 0x84000020, (volatile u32 *)0x040000d4);
    for (i = 0; i != 64; i++, color++) {
        s32 red = *color & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 blue = (*color >> 10) & 0x1f;
        s32 target_red = target[i] & 0x1f;
        s32 target_green = (target[i] >> 5) & 0x1f;
        s32 target_blue = (target[i] >> 10) & 0x1f;

        if (red < target_red)
            red++;
        else if (red > target_red)
            red--;
        if (green < target_green)
            green++;
        else if (green > target_green)
            green--;
        if (blue < target_blue)
            blue++;
        else if (blue > target_blue)
            blue--;
        target[i] = (blue << 10) | (green << 5) | red;
    }
    Dma_Set(target + 1, (void *)0x05000002, 0x8000003f, (volatile u32 *)0x040000d4);
}
