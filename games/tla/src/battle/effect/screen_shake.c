#include "types.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

void Func_08158d68(s32 range)
{
    u8 *fx = Ram_HeapSlots->battle_fx_work[0];
    s32 *timer;

    /* FAKEMATCH: keep the timer address in its own block; the ordinary
     * assignment reorders two instruction pairs when range is unpinned. */
    do {
        timer = (s32 *)(fx + 0x77a8);
    } while (0);
    if (*timer > 0) {
        s32 shake = ((range - 1) & Random16()) - range / 2;

        gBgScroll[1].y = shake + 32;
        gCameraSceneParameters[4] = 120 - shake;
        (*timer)--;
    } else {
        gBgScroll[1].y = *(s32 *)(fx + 0x77a4);
        gCameraSceneParameters[4] = 120;
    }
}
