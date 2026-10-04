#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "RAM_BUFFER.H"

/* One of the three file summaries; a level of 0 marks a free file. */
struct SaveSummary {
    u8 unknown_00[0x1c];
    u8 level;
    u8 unknown_1d[0x23];
};

/* The save workspace in heap slot 204: ☀️'s, with a sector buffer 0x3000
   bytes longer before the summaries. */
struct SaveWorkspace {
    u8 unknown_0000[0x303c];
    struct SaveSummary summary[3];
};

LAYOUT_SIZE_GUARD(SaveSummary_Size, struct SaveSummary, 64);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Summary, struct SaveWorkspace, summary, 0x303c);

/* ☀️'s: the first file whose summary is free, or 999 when all three are
   used. */
u32 SaveState_FindFreeSummarySlot(void)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    struct SaveSummary *p;
    u32 i;

    /* FAKEMATCH: the native issues the workspace load before the summary offset. */
    __asm__("");
    p = work->summary;
    for (i = 0; i < 3; i++, p++) {
        if (p->level == 0)
            return i;
    }
    return 999;
}
