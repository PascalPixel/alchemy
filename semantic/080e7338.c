#include "types.h"

/*
 * Claim the first free slot of a 16-entry request table and fill in three of
 * its fields.  The table lives 0x7400 bytes into the block named by the
 * pointer at base-136, each entry is 28 bytes, and an entry is free when its
 * word at +0x18 holds -1.  Claiming an entry writes 0 over that marker.  When
 * all sixteen entries are taken the owner returns without writing anything.
 *
 * The base register is saved and restored by the prologue but never written,
 * and is read exactly once, at 0x080e7344.  That is the frame-balanced
 * live-in already recorded for resource_3cd:04b0 - a close without its open -
 * so it is converted as an uninitialised local rather than skipped.
 *
 * The two stores of the free marker at 0x080e735c and 0x080e738a are separate
 * instructions that both reach the shared field store at 0x080e738c, so the
 * tail is written once and reached by `goto`.
 *
 * The epilogue at 0x080e7392 ends `pop {r0} ; bx r0`, so the owner is void.
 * One direct caller, 0x080e7c0c.
 */

struct Slot_080e7338 {
    u32 request;
    u32 argument;
    u32 unused_08;
    u32 parameter;
    u32 unused_10;
    u32 unused_14;
    s32 state;
};

void Func_080e7338(u32 request, u32 argument, u32 parameter)
{
    u8 *base;
    struct Slot_080e7338 **table;
    struct Slot_080e7338 *slot;
    s32 index;

    table = (struct Slot_080e7338 **)(base - 136);
    slot = (struct Slot_080e7338 *)((u8 *)*table + 0x7400);
    index = 0;
    if (slot->state == -1) {
        slot->state = 0;
        goto fill;
    }

    for (;;) {
        index++;
        if (index == 16) {
            return;
        }
        slot = (struct Slot_080e7338 *)((u8 *)*table + index * 28 + 0x7400);
        if (slot->state == -1) {
            break;
        }
    }
    slot->state = 0;

fill:
    slot->request = request;
    slot->argument = argument;
    slot->parameter = parameter;
}
