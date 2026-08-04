#include "types.h"

struct TransferState {
    u8 padding0[0x7780];
    s32 mode;
    s32 value;
    u8 padding7788[0x98];
    s32 frame;
    s32 pending;
};

struct TransferContext {
    struct TransferState *state;
    void *source;
};

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

extern struct TransferContext Data_03001eec;

extern void Func_0800562c(void *, void *, u32);
extern void Func_0800567c(void *, void *, u32);

typedef void (*TransferRoutine)(void *, u32, s32);

static inline void StartDma(void *source)
{
    void *destination = (void *)0x06003500;
    u32 control = 0x84002000;
    volatile struct DmaChannel *dma =
        (volatile struct DmaChannel *)0x040000D4;

    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}

void Func_080f60a0(void)
{
    struct TransferState *state = Data_03001eec.state;

    if (state->pending == 1) {
        void *source = Data_03001eec.source;

        switch (state->mode) {
        case 1:
            StartDma(source);
            ((TransferRoutine)0x03000168)(
                source,
                0x8000,
                state->value);
            break;
        case 2:
            if (state->value == 50)
                Func_0800567c(source, (void *)0x06003500, 0x8000);
            else
                Func_0800562c(source, (void *)0x06003500, 0x8000);
            break;
        }

        state->pending = 0;
        state->frame = 1;
    } else {
        state->frame++;
    }
}
