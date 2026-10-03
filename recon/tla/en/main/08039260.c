/*
 * Draft: UiWindow_Create. The native extent is 276 bytes in every edition:
 * JA starts at 08039254, the others at 08039260. Own-ROM comparison finds
 * the same body, with only call displacements differing between editions.
 *
 * Finite ordinary trials (2026-10-03, approved TLA flags):
 * - Legacy private state/self view and ignored x argument: score 220,
 *   3 differing rows. This does not establish the callee's argument contract.
 * - Maintained UiWindow output list, signed duration, guarded window array
 *   and no-argument ResetCounters: score 420, 9 rows; retained.
 * - Branch-local u16 flags read before duration, then flags/frame stores:
 *   score 1005, 16 rows; rejected.
 *
 * Remaining: the no-argument call changes x/width/height register lifetimes
 * and adds one move; the immediate-draw branch still stores flags before
 * duration. ResetCounters does not read incoming r0, so passing x solely
 * to recover its lifetime would be a measured matching device, not an API.
 * No FAKEMATCH device is used; this is not an adoption.
 */
#include "WINDOW.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

void UiWork_ResetCounters(void);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_WaitUntilField1aClear(void *work);

/* Open a window at a tile position and size in the first free record, with
   the attribute bits that choose its frame and drawing; a window drawn
   through its attributes appears at once, any other one opens over eight
   frames. Returns the record, or 0 when all twelve are in use. */

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->windows;
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->duration != 0) {
        i++;
        slot++;
        if (i == UI_WINDOW_COUNT) {
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
        found->output.head = NULL;
        found->state = 0;
        found->output.tail_link = &slot->output.head;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters();
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
            found->duration = 1;
            found->flags |= 2;
            found->frame = 0;
            UiWork_DrawByAttributes(found);
        } else {
            found->duration = 8;
            found->frame = 7;
            UiWork_WaitUntilField1aClear(found);
            WaitFrames(1);
        }
    }
    return found;
}
