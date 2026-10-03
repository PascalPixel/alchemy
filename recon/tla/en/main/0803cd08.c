/*
 * Draft: UiRender_LookupNamedValue; raw/0803cd08.s spans 84 bytes.
 * Search eight u16 names for the full u32 argument; return the paired u32
 * value, or zero when absent. A nonzero clear also clears both entries.
 * Native arrays: JA 0x114c/0x116c; international 0x134c/0x136c. All six checked.
 * Prior headerless private-offset/goto draft used Data_03001e8c and undefined
 * RENDER_*_TBL_OFS macros; no fresh baseline score was captured before this
 * ownership correction.
 * Initial maintained-model/TBS loop: EN 220/3, 82-byte body versus 84-byte
 * listing. Root/offset load and first-name-load/input-copy orders differ;
 * native has two trailing alignment bytes. Sole named-array cursor retry:
 * 1575/36; retained the initial indexed form. All six have the same remaining
 * order changes and extent difference. No complete-extent match.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"

u32 UiRender_LookupNamedValue(u32 name, u32 clear)
{
    struct UiRenderWork *work =
        (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    u32 index;
    u32 value = 0;

    for (index = 0; index < 8; index++) {
        if (work->names[index] == name) {
            value = work->values[index];
            if (clear != 0) {
                work->values[index] = 0;
                work->names[index] = 0;
            }
            break;
        }
    }
    return value;
}
