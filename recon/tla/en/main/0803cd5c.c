/* Near miss of Func_0803cd5c (40 instructions, 2 differ): the native loads the render work before
 * it starts the flag offset 0x1389; a one-pass block after the load moves the parameter copy and
 * the cleared result up instead (4 differ). */
#include "types.h"
#include "ram_buffer.h"

struct UiMessageWait {
    u8 unknown_00[20];
    u16 timer;
};

#include "input.h"
s32 Audio_Check(void);

s32 Func_0803cd5c(struct UiMessageWait *wait)
{
    u8 *work = Ram_HeapSlots->window_tiles;
    s32 done = 0;
    volatile u32 *held; /* FAKEMATCH: the native keeps the address of the held keys and reads them twice (a plain read is shared) */

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the render work before it builds the flag offset (2 differ without it). */

    if (work[0x1389] != 0 && Audio_Check() == 0)
        done = 1;
    held = &gInput.held;
    if (*held & 0x303)
        done = 1;
    if (*held & 2)
        done = 1;
    if (done) {
        wait->timer = 0;
        return 1;
    }
    return 0;
}
