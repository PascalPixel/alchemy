/* Draft: MapAnimation_Update, The Broken Seal's MAP3.C over ⚓️'s MAP.H.
 * Differ 4 (dd): the two loop-invariant VRAM bases 0x06004000 and 0x06008000
 * load later in the prologue than the native code puts them. The decode
 * buffer and block table must fold as constants (Ram_DecodeBuffer and
 * Ram_MapBlocks in ☀️); with labels the allocation drifts (differ 48).
 */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "MAP.H"

extern u8 Data_0201c000[];
extern u8 gMapBlocks[];

void MapAnimation_Update(void)
{
    struct MapWork *state = Ram_HeapSlots->map_work;
    u32 i;

    for (i = 0; i <= 15; i++) {
        const u16 *script;
        u32 op;
        u32 count;
        u32 dst;

        if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
            continue;
    next:
        if ((state->anim + i)->timer == 0) {
            script = (state->anim + i)->cursor;
            op = *script++;
            if (op == 0xffff) {
                (state->anim + i)->cursor = (state->anim + i)->start;
                goto next;
            }
            if ((op & 0xff00) == 0xfe00) {
                if ((op & 0xff) == 0xff)
                    continue;
                (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
                goto next;
            }
            count = *script++;
            dst = script[0];
            (state->anim + i)->timer = script[1];
            if (state->wide_tiles == 0) {
                if (op >= 0x600)
                    Dma_Set(((u8 *)0x0201c000) + op * 32, (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 32 + 0x06004000), (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
            } else {
                if (op >= 0x200)
                    Dma_Set(((u8 *)0x02020000) + op * 64, (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 64 + 0x06008000), (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
            }
            (state->anim + i)->cursor += 4;
            goto next;
        } else {
            (state->anim + i)->timer--;
        }
    }
}
