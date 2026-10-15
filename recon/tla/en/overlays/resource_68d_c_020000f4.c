#include "types.h"
#include "frame.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern s16 Data_02002608[];

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02002024(s32 actor, s32 a, s32 b);

// Every sixth frame rotates colours 1-14 of background palette 12 by one place; every
// frame sets its colour 15 from a twelve-step cycle. The loop is written with goto, as
// overlay 689's Func_020002f8: the bytes rebuild the palette address on every pass.
void Func_020000f4(void)
{
    s32 color;

    if (gFrameCount % 6 == 0) {
        volatile u16 *palette;
        s16 saved;
        s32 index;

        saved = *(volatile u16 *)0x0500019c;
        index = 14;
    loop:
        palette = (volatile u16 *)0x05000180;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto loop;
        *(volatile u16 *)0x05000182 = saved;
    }
    color = Data_02002608[(gFrameCount >> 2) % 12];
    *(volatile u16 *)0x0500019e = color;
}
