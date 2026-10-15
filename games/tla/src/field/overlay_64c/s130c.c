#include "types.h"
#include "io_write_queue.h"

extern u32 gFrameCount;
extern u16 Data_02003fc0;

// Sets the layer blend, switching its alpha every two frames.
void Func_0200130c(void)
{
    u16 *blend;
    u16 blink;
    u16 value;
    u32 reg;

    QueueIoWriteDelay2(0x04000050, 0x3f41);
    reg = 0x04000052;
    blink = gFrameCount & 2;
    blend = &Data_02003fc0;
    if (blink) {
        value = *blend | 12;
        QueueIoWriteDelay2(reg, value);
    } else {
        value = *blend | 16;
        QueueIoWriteDelay2(reg, value);
    }
}
