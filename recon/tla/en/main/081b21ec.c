/*
 * Draft: Palette_StepTowardResource does not yet match; 3 halfwords differ from ☀️'s C, first at +0x26 (ldr r7, [pc, #16]).
 * Links as recon/tla/raw/081b203c.s.
 */
#include "DMA.H"

/* Step background colours 1-63 one unit per channel towards the palette of
   resource_id. */
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
