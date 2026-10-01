/*
 * Draft: UiWindow_Create, ported from its ☀️ twin with ⚓️'s twelve windows
 * 0x508 into the window work in its heap slot. Remaining difference, in the
 * attribute-drawn branch: the listing stores the timer before or-ing 2 into
 * the flags; this C or-s first (score 220; 45 seconds of permuting found
 * nothing better).
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* One of the twelve window records 0x508 into the window work. */
struct UiWindow {
    s32 state;
    struct UiWindow *self;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown_10;
    u16 unknown_12;
    u16 unknown_14;
    u16 flags;
    u16 unknown_18;
    s16 timer;
    u8 unknown_1c[8];
};


void WaitFrames(s32 frames);
void UiWork_ResetCounters();
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_WaitUntilField1aClear(struct UiWindow *window);

/* Open a window at a tile position and size in the first free record, with
   the attribute bits that choose its frame and drawing; a window drawn
   through its attributes appears at once, any other one opens over eight
   frames. Returns the record, or 0 when all twelve are in use. */

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = (struct UiWindow *)(Ram_HeapSlots->window_tiles + 0x508);
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->timer != 0) {
        i++;
        slot++;
        if (i == 12) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->y = y;
        found->width = width;
        found->height = height;
        found->x = x;
        found->state = 0;
        found->unknown_14 = 0;
        found->self = slot;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters(x); /* the reset ignores the x it is passed */
        if (attrs & 8) {
            found->flags |= 8;
        }
        if (attrs & 32) {
            found->flags |= 32;
        }
        if (attrs & 64) {
            found->flags |= 64;
        }
        if (attrs & 128) {
            found->flags |= 128;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->timer = 1;
            found->flags |= 2;
            found->unknown_18 = 0;
            UiWork_DrawByAttributes(found);
        } else {
            found->timer = 8;
            found->unknown_18 = 7;
            UiWork_WaitUntilField1aClear(found);
            WaitFrames(1);
        }
    }
    return found;
}
