#include "types.h"

extern u8 *Func_0808a080(s32 actorId);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);

/*
 * Complete 412-byte actor-ordering owner.  The high-register prologue and
 * 132-byte frame begin at 0x02001be8, the interworking return is at
 * 0x02001d60-0x02001d70, and the alignment plus four-word DMA pool runs
 * through 0x02001d83 before the next prologue.
 *
 * The first three actors in slots 8..10 are compared with all four actors in
 * slots 8..11.  When their depth ordering is inconsistent, DMA3 swaps the
 * complete 112-byte actor records and the corresponding 16-byte metadata
 * payloads.  The fifth word of each 20-byte metadata record is deliberately
 * not moved: it is the story-flag identity attached to the slot, and the
 * following flag hand-off makes the newly occupied slot active.  All ten
 * static calls remain in the exact branch order found in the overlay.
 */

#define DMA3_COPY(source_, destination_, control_)                         \
    do {                                                                  \
        *(volatile u32 *)0x040000d4 = (u32)(source_);                      \
        *(volatile u32 *)0x040000d8 = (u32)(destination_);                 \
        *(volatile u32 *)0x040000dc = (control_);                          \
        while ((*(volatile u32 *)0x040000dc & 0x80000000u) != 0) {        \
        }                                                                 \
    } while (0)

void Func_02001be8(void)
{
    u8 actorScratch[112];
    u8 metadataScratch[16];
    u8 *metadata = (u8 *)0x0200b6d0;
    s32 outer;

    for (outer = 0; outer < 3; outer++) {
        u8 *anchor = Func_0808a080(outer + 8);
        s32 inner;

        for (inner = 0; inner < 4; inner++) {
            u8 *candidate = Func_0808a080(inner + 8);
            u8 *anchorMetadata = metadata + outer * 20;
            u8 *candidateMetadata = metadata + (outer + inner) * 20;
            s32 anchorFlag = *(s32 *)(anchorMetadata + 16);
            s32 candidateFlag = *(s32 *)(candidateMetadata + 16);

            if (*(s32 *)(anchor + 12) <= *(s32 *)(candidate + 12) &&
                *(s32 *)(anchor + 16) < *(s32 *)(candidate + 16)) {
                continue;
            }

            DMA3_COPY(candidate, actorScratch, 0x8400001c);
            DMA3_COPY(anchor, candidate, 0x8400001c);
            DMA3_COPY(actorScratch, anchor, 0x8400001c);

            DMA3_COPY(candidateMetadata, metadataScratch, 0x84000004);
            DMA3_COPY(anchorMetadata, candidateMetadata, 0x84000004);
            DMA3_COPY(metadataScratch, anchorMetadata, 0x84000004);

            if (Func_080770c0(anchorFlag) != 0) {
                if (Func_080770c0(candidateFlag) == 0) {
                    Func_080770d0(anchorFlag);
                    Func_080770c8(candidateFlag);
                }
            } else if (Func_080770c0(anchorFlag) == 0) {
                if (Func_080770c0(candidateFlag) != 0) {
                    Func_080770c8(anchorFlag);
                    Func_080770d0(candidateFlag);
                }
            }
        }
    }
}

#undef DMA3_COPY
