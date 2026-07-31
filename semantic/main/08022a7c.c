#include "types.h"

/*
 * Allocate a sprite record, give it its fixed attributes, and place it
 * relative to the view named by the pointer four bytes below the context
 * base.
 *
 * Func_08015e8c hands back the record, or nothing at all, in which case the
 * owner returns without doing anything.  A new record is marked live at +4
 * and +5, given the animation handle Func_080040b4(128) returns, priority
 * 0xf0 at +0x0f and the neutral position 120 at +6 and +8.
 *
 * The attribute block starts at +0x10.  Its word at +0x14 is set to
 * 0x40000400 and its word at +0x18 cleared, and then three packed fields are
 * written over that: the nine-bit x at +0x16 from the view's halfword at
 * +0x0c scaled by eight plus the caller's x, the byte y at +0x14 from the
 * view's halfword at +0x0e scaled by eight plus the caller's y, and the
 * ten-bit tile at +0x18 from Func_080040d0.  The caller's third argument
 * chooses which of the two frame sets at 0x080313a4 and 0x08031424 that call
 * resolves against.
 *
 * The record is finally attached to the view by Func_08016584.
 *
 * The context base is saved and restored by the prologue but never written,
 * and is read at 0x08022a8e and 0x08022ac6.  That is the frame-balanced
 * live-in recorded for resource_3cd:04b0 - a close without its open - and
 * converts as an uninitialised local rather than a skip.
 *
 * The epilogue at 0x08022b30 ends `pop {r0} ; bx r0`, so the owner is void.
 * Six direct callers, the first at 0x08022ff4.
 */

void *Func_08015e8c(void);
s32 Func_080040b4(s32 kind);
s32 Func_080040d0(s32 handle, const void *frames);
void Func_08016584(void *view, void *record);

struct View_08022a7c {
    u8 unknown_00[0xc];
    u16 origin_x;
    u16 origin_y;
};

void Func_08022a7c(s32 x, s32 y, s32 alternate)
{
    u8 *context;
    struct View_08022a7c **view_slot;
    u8 *record;
    u8 *attributes;
    s32 handle;
    s32 tile;

    record = (u8 *)Func_08015e8c();
    if (record == NULL) {
        return;
    }

    record[5] = 1;
    record[4] = 1;
    handle = Func_080040b4(128);
    record[15] = 240;
    *(u16 *)(record + 6) = 120;
    *(u16 *)(record + 8) = 120;
    record[14] = (u8)handle;

    attributes = record + 16;
    *(u32 *)(attributes + 4) = 0x40000400;
    *(u32 *)(attributes + 8) = 0;

    view_slot = (struct View_08022a7c **)(context - 4);
    *(u16 *)(attributes + 6) =
        (u16)((*(u16 *)(attributes + 6) & 0xfe00)
              | (((*view_slot)->origin_x * 8 + x) & 0x1ff));
    attributes[4] = (u8)((*view_slot)->origin_y * 8 + y);

    handle = record[14];
    tile = Func_080040d0(handle,
                         alternate != 0 ? (const void *)0x080313a4
                                        : (const void *)0x08031424);
    *(u16 *)(attributes + 8) =
        (u16)((*(u16 *)(attributes + 8) & 0xfc00) | (tile & 0x3ff));

    Func_08016584(*view_slot, record);
}
