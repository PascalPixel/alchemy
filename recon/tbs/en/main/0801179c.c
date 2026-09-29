/* Draft, not exact (2026-09-27): 12 differing halfwords, 11 aligned edits,
   268 of 268 bytes including the complete pool through 080118a8.
   The loop body matches. Both versions retain the VRAM base in ip and
   0xffff in lr; the differing ancestry is the prologue's r2/r3 loads and
   their pool slots, plus scheduling of the animation pointer setup.

   Bounded named-address experiment: the exact StartChannels neighbour
   confirms 16 twelve-byte channels and the halfword command stream;
   Animation_Start confirms the second VRAM character block. Replacing
   all three numeric 06004000 address expressions with the named array
   below produced bytes identical to the baseline, not a new match.
   Fresh loop dumps still hoist sentinel pseudo 47 at insn 443 before
   VRAM pseudo 84 at 447; the latter now has a symbol_ref REG_EQUAL but
   unchanged liveness and allocation. The timer decrement shares pseudo
   47 with the command sentinel. A script wrapper has no demonstrated
   ownership lever here, so no wrapper or declaration/width sweep was
   attempted. Retain this address clarification and stop this axis.
   2026-09-29 alchemy permute (seed 1, 4 jobs, 10 minutes): 31,884
   candidates, none below the draft's score 260 (4 operand, 3 reordered),
   22,654 level with it. The build names the EWRAM buffers gMapWork,
   gDecodeBuffer and gMapBlocks, but every spelling of the two sources
   through those names (symbol plus op * 32, op * 32 plus symbol, array
   element, word array) scores 490: the named base allocates the script
   registers differently from the literal the draft uses, so the draft's
   literal EWRAM addresses, which block adoption, are also what matches
   here. Inner while, while (timer == 0) and early-continue loop forms score
   990 to 1685. */

#include "TYPES.H"
#include "DMA.H"

struct MapAnimation {
    u16 *start;
    u16 *cursor;
    u16 timer;
    u16 paused;
};

extern u8 *gMapWork;
extern u8 Data_06004000[];

void MapAnimation_Update(void)
{
    u8 *work = gMapWork;
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
                    Dma_Set((void *)(op * 32 + 0x0201c000), &Data_06004000[dst * 32], (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set(&Data_06004000[op * 32], &Data_06004000[dst * 32], (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
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
