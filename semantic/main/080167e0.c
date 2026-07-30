typedef signed int s32;
typedef unsigned int u32;

struct DmaTransfer {
    const void *source;
    void *target;
    u32 control;
};

void Func_080072f0(s32, s32, s32, s32);

void Func_080167e0(s32 arg0)
{
    s32 source = 0x06002520;
    s32 width = arg0 * 6;
    s32 offset = arg0 * 24;
    s32 shifted = source + offset;
    s32 target = 0x06002500;
    s32 row = 29;

    do {
        struct DmaTransfer *transfer = (struct DmaTransfer *)0x040000d4;
        transfer->source = (const void *)shifted;
        transfer->target = (void *)source;
        transfer->control = (24 - width) | 0x84000000;
        Func_080072f0(target + (32 - width) * 4, offset, 0, 0x03000168);
        row--;
        target += 128;
        source += 128;
        shifted += 128;
    } while (row >= 0);
}
