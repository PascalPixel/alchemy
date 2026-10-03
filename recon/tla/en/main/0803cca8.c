/*
 * Draft: UiWork_ClearValueNameTables; raw/0803cca8.s spans 40 bytes.
 * The eight u32 values and u16 names are cleared together, value first.
 * Native arrays: JA 0x114c/0x116c; international 0x134c/0x136c. All six checked.
 * Prior private-pointer UiWindow_ClearSlots form: EN 510/16; its old header
 * incorrectly named raw/0803cba8.s. Retained trial; not a match claim.
 * Initial maintained-model/TBS loop: EN 160/2, 38-byte body versus 40-byte
 * listing. Root load and offset-constant load are reversed; native has two
 * trailing alignment bytes. Sole named-array cursor retry: 495/14; retained
 * the initial indexed form. All six have the same remaining order changes
 * and extent difference. No new device or complete-extent match.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"

void UiWork_ClearValueNameTables(void)
{
    s32 no;
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    no = 0;
    do {
        work->values[no] = 0;
        work->names[no] = 0;
        no++;
    } while (no != 8);
}
