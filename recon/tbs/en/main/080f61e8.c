/* Draft, not exact (2026-09-24): 84 differing halfwords, 204 of 208 bytes.
   Residual: the reference emits its first literal pool (0x1f, DMA3,
   0x84000020) right after the first transfer and branches over it into the
   fade loop; the green and blue masks load 0x1f from that pool while the
   red mask is an immediate, hence the link symbol. */

#include "TYPES.H"
#include "DMA.H"

void *Func_08002f40(s32 id);
extern u8 Value_0000001f[];

void Func_080f61e8(s32 id)
{
    u16 *palette = (u16 *)0x05000000;
    u16 buf[64];
    s32 i;

    Dma_Set(Func_08002f40(id), buf, 0x84000020, (volatile u32 *)0x040000d4);
    for (i = 0; i != 64; i++) {
        s32 color = *palette;
        s32 r = color & 31;
        s32 g = ((u16)color >> 5) & (s32)Value_0000001f;
        s32 b = ((u16)color >> 10) & (s32)Value_0000001f;
        s32 goal = buf[i];
        s32 gr = goal & 31;
        s32 gg = ((u16)goal >> 5) & (s32)Value_0000001f;
        s32 gb = ((u16)goal >> 10) & (s32)Value_0000001f;

        if (r < gr)
            r++;
        else if (r > gr)
            r--;
        if (g < gg)
            g++;
        else if (g > gg)
            g--;
        if (b < gb)
            b++;
        else if (b > gb)
            b--;
        buf[i] = (b << 10) | (g << 5) | r;
        palette++;
    }
    Dma_Set(buf + 1, (void *)0x05000002, 0x8000003f, (volatile u32 *)0x040000d4);
}
