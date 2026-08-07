#include "types.h"

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

struct GraphicsWork {
    u8 pixels[0x600];
    u16 width;
    u16 height;
    const void *source;
};

void *Func_080048b0(s32 resource, s32 size);
void Func_08002dd8(s32 resource);
u16 *Func_08002f40(s32 index);
s32 Func_08004080(void);
s32 Func_08003fa4(s32 handle, s32 size, const void *source);
void Func_0801a5a4(struct GraphicsWork *work, s32 alternate);
extern u8 Data_000000f0[];

void Func_0801a4fc(
    u32 entry,
    s32 unused,
    s32 *handle,
    s32 *upload,
    u32 palette_slot,
    s32 reuse_handle)
{
    u16 *table;
    u16 *palette;
    struct GraphicsWork *work;
    u32 index;

    work = Func_080048b0(17, 0x608);
    table = Func_08002f40((s32)Data_000000f0);
    if (entry <= 0x7f) {
        index = entry;
    } else {
        index = entry;
        index -= 0x70;
    }
    palette = (u16 *)((u8 *)table + table[index]);

    work->source = palette + 16;
    work->width = 4;
    work->height = 4;
    Func_0801a5a4(work, 0);

    if (reuse_handle == 0)
        *handle = Func_08004080();
    *upload = Func_08003fa4(*handle, 0x200, &work->pixels[0x400]);
    Func_08002dd8(17);

    {
        void *destination = (void *)(0x05000200 + palette_slot * 32);

        *(volatile struct DmaChannel *)0x040000d4 = (struct DmaChannel){
            palette, destination, 0x80000010,
        };
    }
}

void Func_0801a5a0(void)
{
}
