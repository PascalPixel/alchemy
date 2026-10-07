#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "RAM_BUFFER.H"
#include "BATTLE_PARTY.H"
#include "B5_CONTEXT.H"

struct SaveSummary {
    u8 unknown_00[28];
    u8 level;
    u8 unknown_1d[35];
};

struct SaveWorkspace {
    u8 unknown_0000[0x303c];
    struct SaveSummary summary[3];
};

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
