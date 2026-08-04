#include "types.h"

struct RenderStream_080118d8 {
    const u16 *start;
    const u16 *current;
    u16 delay;
    u16 alternate;
};

struct RenderState_080118d8 {
    u8 padding000[24];
    struct RenderStream_080118d8 streams[16];
};

extern struct RenderState_080118d8 *Data_03001e70;

void Func_080041d8(void (*callback)(void), s32 period);
void Func_0801179c(void);

void Func_080118d8(const u16 *script)
{
    struct RenderState_080118d8 *state = Data_03001e70;
    u32 zero;
    volatile u32 *dma;
    u32 source;
    u32 destination;
    u32 control;
    u32 configured = 0;
    u16 command;

    zero = 0;
    dma = (volatile u32 *)0x040000d4;
    source = (u32)&zero;
    destination = (u32)state->streams;
    control = 0x85000030;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    command = *script++;
    while (command != 0xffff) {
        if ((command & 0xff00) == 0xfd00) {
            struct RenderStream_080118d8 *stream =
                &state->streams[command & 0x0f];

            stream->start = script;
            stream->current = script;
            stream->delay = 0;
            stream->alternate = (command & 0x80) != 0;
            configured++;
        }
        command = *script++;
    }

    if (configured != 0) {
        Func_080041d8(Func_0801179c, 3200);
    }
}
