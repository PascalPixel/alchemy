/* Draft, differ 47 of 66: ☀️'s Clear_UpdateBlend for overlay 64A. Left:
 * register choice: the native keeps the level in r5, the queue in r1, the
 * interrupt register in r0 and the saved IME in r4; this C takes r4, r0, r1
 * and r5. Tried: a u16, s32 or u32 level, the declarations either way round,
 * the frame incremented in place. */
#include "types.h"
#include "io_write_queue.h"

extern u16 Data_020024f4;

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void Func_0200015c(void)
{
    s32 frame;
    u32 level;

    frame = Data_020024f4 + 1;
    Data_020024f4 = frame;
    level = (u16)frame >> 1;
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x2e51, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000052, ((16 - level) << 8) | level, 0x20000);
    if (level > 15)
        Scheduler_RemoveCallbackFar(Func_0200015c);
}
