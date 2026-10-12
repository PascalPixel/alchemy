/* Near miss of Func_0803cdb4 (49 instructions, 2 differ), the sibling of Func_0803cd5c: the
 * native loads the render work before it starts the flag offset 0x1389. */
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

struct UiMessageWait {
    u8 unknown_00[20];
    u16 timer;
};

s32 Audio_Check(void);

s32 Func_0803cdb4(struct UiMessageWait *wait)
{
    u8 *work = Ram_HeapSlots->window_tiles;
    s32 done = 0;
    u32 keys;

    if (work[0x1389] != 0 && Audio_Check() == 0)
        done = 1;
    keys = gInput.pressed;
    if (work[4] != 0)
        keys = gInput.pending_pressed;
    if (keys & 0x303)
        done = 1;
    if (gInput.held & 2) {
        if (wait->timer == 1)
            done = 1;
    } else {
        wait->timer = 2;
    }
    if (done) {
        wait->timer = 0;
        return 1;
    }
    return 0;
}
