/* Draft, not exact (2026-09-24): 12 differing halfwords, 268 of 268 bytes.
   Residual: the loop-invariant 0xffff and 0x06004000 swap registers and
   literal slots (reference: 0x06004000 in r2 then ip, 0xffff in r3 then lr). */

#include "TYPES.H"
#include "DMA.H"

struct MapAnimation {
    u16 *start;
    u16 *cursor;
    u16 timer;
    u16 paused;
};

extern u8 *Data_03001e70;

void MapAnimation_Update(void)
{
    u8 *work = Data_03001e70;
    struct MapAnimation *anim = (struct MapAnimation *)(work + 24);
    u32 i;

    for (i = 0; i <= 15; i++, anim++) {
        u16 *script;
        u32 op;
        u32 count;
        u32 dst;

        if (anim->start == NULL || anim->paused != 0)
            continue;
    next:
        if (anim->timer == 0) {
            script = anim->cursor;
            op = *script++;
            if (op == 0xffff) {
                anim->cursor = anim->start;
                goto next;
            }
            if ((op & 0xff00) == 0xfe00) {
                if ((op & 0xff) == 0xff)
                    continue;
                anim->cursor = anim->start + (op & 0xff) * 2;
                goto next;
            }
            count = *script++;
            dst = script[0];
            anim->timer = script[1];
            if (work[22] == 0) {
                if (op >= 0x600)
                    Dma_Set((void *)(op * 32 + 0x0201c000), (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 32 + 0x06004000), (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
            } else {
                if (op >= 0x200)
                    Dma_Set((void *)(op * 64 + 0x02020000), (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 64 + 0x06008000), (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
            }
            anim->cursor += 4;
            goto next;
        } else {
            anim->timer--;
        }
    }
}
