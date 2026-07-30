#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct DmaTransfer_080056cc {
    const void *source;
    void *destination;
    u32 control;
};

struct ProbeBuffer_080056cc {
    u8 active[16];
    u8 category[16];
    u16 key[16];
    u8 work[0x10c0];
};

typedef char Assert_080056cc_category[
    OFFSET_OF(struct ProbeBuffer_080056cc, category) == 0x10 ? 1 : -1];
typedef char Assert_080056cc_key[
    OFFSET_OF(struct ProbeBuffer_080056cc, key) == 0x20 ? 1 : -1];
typedef char Assert_080056cc_work[
    OFFSET_OF(struct ProbeBuffer_080056cc, work) == 0x40 ? 1 : -1];
typedef char Assert_080056cc_size[
    sizeof(struct ProbeBuffer_080056cc) == 0x1100 ? 1 : -1];

void *Func_080048f4(s32 kind, s32 size);
s32 Func_080069c8(s32 timer, void *vector);
s32 Func_08006910(void);
void Func_080030f8(s32 frames);
s32 Func_080058ac(u16 index);
s32 Func_08005c08(u8 *left, u8 *right, s32 count);

s32 Func_080056cc(void)
{
    volatile struct DmaTransfer_080056cc *dma =
        (volatile struct DmaTransfer_080056cc *)0x040000d4;
    struct ProbeBuffer_080056cc *buffer;
    u32 zero = 0;
    s32 attempt;
    s32 index;

    buffer = Func_080048f4(51, 0x1100);

    dma->source = &zero;
    dma->destination = buffer;
    dma->control = 0x85000440;

    Func_080069c8(2, (void *)0x030000f4);

    for (attempt = 0; attempt <= 7; attempt++) {
        if (Func_08006910() == 0)
            break;
        Func_080030f8(1);
    }
    if (attempt > 7)
        return 1;

    for (index = 0; index < 16; index++) {
        u16 sample[8];
        s32 offset;
        u8 category;
        s32 prior;

        buffer->active[index] = 0;
        buffer->category[index] = 16;
        buffer->key[index] = 0;

        offset = Func_080058ac(index);

        dma->source = buffer->work;
        dma->destination = sample;
        dma->control = 0x84000004;
        while (dma->control & 0x80000000) {
        }

        if (Func_08005c08((u8 *)sample, (u8 *)0x080079b0, 7) != 0)
            continue;

        buffer->key[index] = sample[5];
        category = ((u8 *)sample)[7];
        if (category > 15 || offset != 0)
            continue;

        buffer->active[index] = 1;
        buffer->category[index] = category;

        for (prior = 0; prior < index; prior++) {
            if (buffer->category[prior] != category)
                continue;

            if (buffer->key[prior] < buffer->key[index])
                buffer->active[prior] = 0;
            else
                buffer->active[index] = 0;
        }
    }

    return 0;
}
