#ifndef ALCHEMY_PALQUEUE_H
#define ALCHEMY_PALQUEUE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* A palette cycle keeps its original colours and its next rotation. */
struct PaletteSnapshot {
    u16 *source;
    u16 position;
    u16 timer;
    s16 delay;
    u16 count;
    u16 colors[16];
};

/* Heap slot 28 holds four cycles and the number queued. */
struct PaletteSnapshotWork {
    struct PaletteSnapshot entries[4];
    u16 count;
};

LAYOUT_SIZE_GUARD(PaletteSnapshot_Size, struct PaletteSnapshot, 0x2c);
LAYOUT_OFFSET_GUARD(PaletteSnapshot_Position, struct PaletteSnapshot, position, 4);
LAYOUT_OFFSET_GUARD(PaletteSnapshot_Timer, struct PaletteSnapshot, timer, 6);
LAYOUT_OFFSET_GUARD(PaletteSnapshot_Delay, struct PaletteSnapshot, delay, 8);
LAYOUT_OFFSET_GUARD(PaletteSnapshot_Colors, struct PaletteSnapshot, colors, 0xc);
LAYOUT_SIZE_GUARD(PaletteSnapshotWork_Size, struct PaletteSnapshotWork, 0xb4);
LAYOUT_OFFSET_GUARD(PaletteSnapshotWork_Count, struct PaletteSnapshotWork, count, 0xb0);

void Runtime_AllocateAndClearQueue(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);
void Runtime_ScheduleCallbackAndReleaseBlock28(void);
void Func_08011bf4(void);

#endif
