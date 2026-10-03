/*
 * Draft: UiWork_PushValueSlot; raw/0803ccd0.s spans 56 bytes.
 * Store the full value and truncated u16 name in the first unnamed slot;
 * leave a full eight-entry table unchanged. The native loop leaves its
 * index in r0, but audited main and overlay callers discard it. Retain the maintained
 * TBS void contract; the residual register does not prove a return type.
 * Native arrays: JA 0x114c/0x116c; international 0x134c/0x136c.
 * Initial maintained-model/TBS loop: EN 100/1, solely the two trailing
 * alignment bytes. All six compiled 54-byte bodies equal their native
 * instructions. No source padding, complete-extent match or adoption claimed.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"

void UiWork_PushValueSlot(u32 value, u32 name)
{
    struct UiRenderWork *work =
        (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    u32 no = 0;
    u32 limit = 8;

    do {
        if (work->names[no] == 0) {
            work->values[no] = value;
            work->names[no] = name;
            break;
        }
        no++;
    } while (no != limit);
}
