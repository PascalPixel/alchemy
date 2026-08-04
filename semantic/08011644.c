#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_08011644 {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_08011644;

typedef struct MapInitWork_08011644 {
    u8 padding000[0xfc];
    u8 active;
    u8 padding0fd[3];
    s16 first_row;
    s16 last_row;
    u8 padding104[0x18];
    s32 *resources;
} MapInitWork_08011644;

LAYOUT_OFFSET_GUARD(
    MapInitWork08011644_Active,
    MapInitWork_08011644,
    active,
    0xfc);
LAYOUT_OFFSET_GUARD(
    MapInitWork08011644_FirstRow,
    MapInitWork_08011644,
    first_row,
    0x100);
LAYOUT_OFFSET_GUARD(
    MapInitWork08011644_Resources,
    MapInitWork_08011644,
    resources,
    0x11c);

extern MapInitWork_08011644 *Data_03001e70;
extern void (*Data_03001cfc)(void);

s32 Func_08002f40(s32 resource);
void Func_080030f8(u32 frames);
void Func_0800439c(u32 callback);
void Func_08005340(s32 source, void *destination);
void Func_080053e8(s32 source, void *destination);
void Func_080113e4(void);
void Func_0801161c(void);
void Func_0801179c(void);

/* Stage the map palettes/assets and install the one-frame upload callback. */
void Func_08011644(void)
{
    volatile DmaChannel_08011644 *dma =
        (volatile DmaChannel_08011644 *)0x040000d4;
    MapInitWork_08011644 *work = Data_03001e70;
    void *palette_buffer = (void *)0x02010000;
    s32 *resources = work->resources;
    u16 saved_backdrop = *(volatile u16 *)0x05000000;

    Func_080053e8(Func_08002f40(resources[0]), palette_buffer);
    *(u16 *)palette_buffer = saved_backdrop;
    dma->source = palette_buffer;
    dma->destination = (void *)0x05000000;
    dma->control = 0x84000070;

    Func_08005340(Func_08002f40(resources[1]), (void *)0x02038000);
    Func_08005340(Func_08002f40(resources[2]), (void *)0x0203a000);
    Func_08005340(Func_08002f40(resources[3]), (void *)0x0203c000);
    Func_08005340(Func_08002f40(resources[4]), (void *)0x0203e000);

    Data_03001cfc = Func_0801161c;
    work->first_row = 0;
    work->last_row = 0x9f;
    Func_080030f8(1);

    Func_08005340(Func_08002f40(0xd5), palette_buffer);
    Func_080113e4();
    work->active = 0;
    Func_0800439c((u32)Func_0801179c);
    Func_080030f8(1);
}
