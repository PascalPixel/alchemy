#include "types.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

void Func_08158d68(s32 range)
{
    u8 *fx = Ram_HeapSlots->battle_fx_work[0];
    s32 *timer;

    {
        /* FAKEMATCH: ⚓️ keeps the range in r0 until it has loaded the effect work */
        register s32 incoming asm("r0") = range;

        /* FAKEMATCH: and copies it only then */
        asm("" : "+l"(incoming) : "l"(fx));
        range = incoming;
    }
    timer = (s32 *)(fx + 0x77a8);
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
