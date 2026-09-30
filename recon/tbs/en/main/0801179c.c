/* 2026-09-30 (Mercury): EXACT, 268 of 268 bytes with stock agscc, no
   FAKEMATCH. It sits between FIELD/COMMON/MAP modules (after
   LOAD_DEFAULT_CELLS_AND_UPDATE_BLOCK, before MAP3), so its module is
   Mars's to choose; compile it under #if defined(TBS_EDITION_EN) until the
   other editions adopt theirs. Every channel access is spelled (state->anim
   + i)->field: the repeated address arithmetic keeps loop.c's first pass
   over its threshold so the 0xffff sentinel is hoisted only in the rerun
   (state->anim[i].field leaves 165 differences). MAP3.C calls the +10
   halfword looping and has its own struct MapState; reconcile the two on
   adoption. */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"

/* One of the sixteen tile-animation channels in the map state. */
struct MapAnimation {
    u16 *start;
    u16 *cursor;
    u16 timer;
    u16 paused;
};

struct MapState {
    u8 unknown_00[22];
    /* 0: the map's characters are 4 bpp in the second character block;
       otherwise 8 bpp in the third. */
    u8 wide_tiles;
    u8 unknown_17;
    struct MapAnimation anim[16];
};

extern struct MapState *gCam;

/* Steps the sixteen tile-animation channels. A channel whose timer has run
   out reads commands until one copies characters: 0xffff restarts the
   channel, 0xfeXX jumps to command XX (0xfeff stops it for this frame), and
   any other command copies COUNT characters from character OP to character
   DST and waits TIMER frames. Characters past VRAM's range come from EWRAM. */
void MapAnimation_Update(void)
{
    struct MapState *state = gCam;
    u32 i;

    for (i = 0; i <= 15; i++) {
        u16 *script;
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
                    Dma_Set(Ram_DecodeBuffer + op * 32, (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 32 + 0x06004000), (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
            } else {
                if (op >= 0x200)
                    Dma_Set(Ram_MapBlocks + op * 64, (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
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
