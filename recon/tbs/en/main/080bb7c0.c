/* NONMATCHING: main [080bb7c0,080bb8d8), 280 bytes with both pools.
 * 2026-10-01 (☀️ matcher 1): rewritten as plain C from the listing; 276 of
 * 280 bytes, 17 aligned differing lines, permuter score 895 (the previous
 * permuter-built body scored 1550). A while (1) loop with WaitFrames at its
 * end gives the reference's layout (exit test, wait, branch back) and lets
 * the loop hoist its invariants; the goto loop hoisted nothing. The blend
 * value goes through a local so the reference's in-loop movs #16 stays.
 * Remaining: the reference hoists the x coordinate as a halfword load of
 * pos.x (mov r2, sp; ldrh r2, [r2, #0] into r9) and adds 0xfffc for the -4,
 * so it computes x in 16 bits; here pos.x is reloaded as a word inside the
 * loop and -4 is a subs, which moves tiles from fp to r9 and the constant 4
 * and 0x0400004a into other registers. (u16) casts of the sum give the
 * 0xfffc but keep the word load (19 lines); reading pos.x through a u16
 * local before the loop, a u16 view of pos.x, or u16 fields in pos are
 * worse (57 to 79). Ui_GetTableWordZeroFar is the existing veneer at
 * 080153f0. */
#include "SYSTEM.H"
#include "UI.H"
#include "IO_WRITE_QUEUE.H"

struct PromptSprite {
    s32 next;
    u32 y : 8;
    u32 affine : 2;
    u32 mode : 2;
    u32 mosaic : 1;
    u32 colors : 1;
    u32 shape : 2;
    u32 x : 9;
    u32 affine_index : 5;
    u32 size : 2;
    u32 tile : 10;
    u32 priority : 2;
    u32 palette : 4;
    u32 unused : 16;
};

struct PromptPoint {
    s32 x;
    s32 y;
};

extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

s32 Ui_GetTableWordZeroFar(s32 index);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 index, s32 source);
s32 Resource_ResetEntry(u32 index);
void Runtime_PushSlotEntry(s32 *entry, s32 priority);

/* Shows the bobbing prompt arrow at (x, y) until A, B, L or R. */
s32 Unnamed_080bb7c0(s32 x, s32 y)
{
    struct PromptPoint pos;
    s32 tiles;
    struct PromptSprite entry;
    struct PromptSprite *sprite;
    s32 slot;
    s32 alpha;

    tiles = Ui_GetTableWordZeroFar(0);
    pos.x = x;
    pos.y = y;
    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    sprite = &entry;
    slot = Resource_LoadIntoFreeSlot(0x80);
    while (1) {
        alpha = 16;
        QueueIoWriteDelay10(0x0400004a, 4);
        QueueIoWriteDelay6(0x0400004a, 16);
        *(volatile u16 *)0x04000052 = alpha;
        ((s32 *)sprite)[1] = 0x40000000;
        ((s32 *)sprite)[2] = 0;
        sprite->tile = Resource_GetBuffer(slot, tiles);
        sprite->x = ((gFrameCount & 4) >> 1) + pos.x - 4;
        sprite->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        Runtime_PushSlotEntry((s32 *)sprite, 240);
        if (gKeyState & 0x303)
            break;
        WaitFrames(1);
    }
    Resource_ResetEntry(slot);
    WaitFrames(1);
}
