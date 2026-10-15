/* Draft, differ 2 of 39: the native loads the save workspace pointer before
 * the prologue's sub sp, #60, this adjusts the stack first. Measured: the
 * slot read plainly, in a one-pass block, before a barrier (differ 10) or
 * through an operand barrier (all differ 2 or worse). The Lost Age checks
 * every third of its 15 slots and answers 15 for none. */
#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"

u32 Random16(void);

u32 SaveState_SelectWriteSlot(s32 mode)
{
    u32 empty[15];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = ((struct SaveWorkspace *)Ram_HeapSlots->serialWork)->occupied;
    count = 0;
    index = 0;
    do {
        value = *slot;
        slot += 3;
        if (value == 0)
            empty[count++] = index;
        index += 3;
    } while (index <= 14);
    index = 15;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (SaveState_FindLatestSlot(mode) == 15)
                index = 15;
        } else {
            index = Random16() % count;
            index = empty[index];
        }
    }
    return index;
}
