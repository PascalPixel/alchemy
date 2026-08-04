#include "types.h"

struct DmaChannel_080c02a4 {
    const void *source;
    void *destination;
    u32 control;
};

struct DeferredTransfers_080c02a4 {
    u16 count;
    u16 unknown_02;
    struct {
        const void *source;
        void *destination;
        u32 control;
    } entry[32];
};

struct EffectState_080c02a4 {
    u8 unknown_00[8];
    s32 mode;
    s32 active;
    s32 phase;
};

struct DisplayWorkspace_080c02a4 {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    u16 ids[24];
};

extern struct EffectState_080c02a4 *Data_03001f00;
extern u8 *Data_03001e74;

void Func_08002dd8(s32);
void Func_0800307c(s32, s32, const void *);
void Func_080030f8(s32);
void Func_0800387c(volatile void *, s32);
void Func_0800393c(volatile void *, s32);
void Func_080039fc(volatile void *, s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
s32 *Func_080048b0(s32, s32);
void Func_08015128(u8);
void *Func_08077008(s32);
void Func_080b595c(s32);
s32 Func_080b6c08(s32, u16 *);
void Func_080b7b6c(u16 *, s32);
void *Func_080b7dd0(s32);
void Func_080c0cec(s32, s32, s32, s32);
void Func_080c0f98(u16, s32);
void Func_080c9028(void *);

static void StartDma_080c02a4(
    const void *source,
    void *destination,
    u32 control)
{
    volatile struct DmaChannel_080c02a4 *dma =
        (volatile struct DmaChannel_080c02a4 *)0x040000d4;

    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}

static void QueueTransfer_080c02a4(
    const void *source,
    void *destination,
    u32 control)
{
    volatile struct DeferredTransfers_080c02a4 *transfers =
        (volatile struct DeferredTransfers_080c02a4 *)0x02002090;
    u16 interrupt_state = *(volatile u16 *)0x04000208;

    *(volatile u16 *)0x04000208 = 0x0208;
    if (transfers->count <= 31) {
        u16 index = transfers->count++;

        transfers->entry[index].source = source;
        transfers->entry[index].destination = destination;
        transfers->entry[index].control = control;
    }
    *(volatile u16 *)0x04000208 = interrupt_state;
}

void Func_080c02a4(s32 resource, s32 mode)
{
    struct EffectState_080c02a4 *state = Data_03001f00;
    s32 *work = Func_080048b0(42, 4);

    if (mode != 0x15b) {
        u32 fill;
        s32 row;
        s32 column;

        StartDma_080c02a4(
            (const void *)0x080c5b30,
            (void *)0x06005020,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5b50,
            (void *)0x06005040,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5b70,
            (void *)0x06005060,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5b90,
            (void *)0x06005080,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5bb0,
            (void *)0x060050a0,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5bd0,
            (void *)0x060050c0,
            0x84000008);
        StartDma_080c02a4(
            (const void *)0x080c5bf0,
            (void *)0x060050e0,
            0x84000008);

        *(volatile u16 *)0x04000000 = 1;
        state->active = 1;
        state->mode = 1;
        state->phase = 0;

        fill = 0x33333333;
        StartDma_080c02a4(&fill, (void *)0x06005000, 0x85000008);
        fill = 0;
        StartDma_080c02a4(&fill, (void *)0x06005100, 0x85000008);

        *(volatile u16 *)0x0400000a = 0x0c04;
        *(volatile u16 *)0x04000008 |= 2;
        state->mode = 2;

        for (row = 0; row < 32; row++) {
            u16 tile = row <= 20 ? 0xf080 : 0xf088;

            for (column = 0; column < 32; column++)
                ((volatile u16 *)0x06006000)[row * 32 + column] = tile;
        }

        *(volatile u16 *)0x03001ad2 = 32;
        *(volatile u16 *)0x03001ad6 = 32;
        *(volatile u16 *)0x03001ad4 = 8;
        Func_080030f8(1);

        *(volatile u16 *)0x04000040 = 0x00f0;
        *(volatile u16 *)0x04000044 = 0x0088;
        *(volatile u16 *)0x04000042 = 0x00f0;
        *(volatile u16 *)0x04000046 = 0x0088;
        *(volatile u16 *)0x04000048 = 0x3537;
        *(volatile u16 *)0x0400004a = 0x3f21;
        Func_0800387c((volatile void *)0x04000000, 0x7741);
        Func_080c0cec(0, 0, 0, 180);

        *work = 0;
        Func_080041d8((const void *)0x080c01bd, 0x0c80);
        Func_080041d8((const void *)0x080c0229, 0x0480);
        Func_0800307c(2, 32, (const void *)0x080c0299);
        *(volatile u16 *)0x03001ad2 = 32;
        Func_080030f8(1);
        Func_08015128(Data_03001e74[0x41]);
        Func_080030f8(20);
        Func_080039fc((volatile void *)0x04000008, 2);
        Func_0800393c((volatile void *)0x04000008, 0);
        Func_080b595c(resource);
        Func_08004278((volatile void *)0x080c01bd);
        Func_08004278((volatile void *)0x080c0229);
        *(volatile u16 *)0x03001ad2 = 0;
        Func_0800307c(2, 0, 0);
    } else {
        struct DisplayWorkspace_080c02a4 display;
        u16 owners[32];
        u16 visible[32];
        s32 owner_count;
        s32 visible_count;
        s32 index;

        state->active = 1;
        state->phase = 0;

        owner_count = Func_080b6c08(3, owners);
        for (index = 0; index < owner_count; index++) {
            s32 owner = index <= 7 ? index : index + 120;
            u8 *owner_data = Func_08077008(owner);
            u8 *metadata = Func_080b7dd0(owner);

            if (owner_data[0x128] != 0x94)
                *(s32 *)(metadata + 0x18) = 0xb333;
        }

        QueueTransfer_080c02a4(
            (const void *)0x00006041,
            (void *)0x04000000,
            0x00020000);
        Func_080030f8(1);

        *(u16 *)(Data_03001e74 + 0x648) = 0x21;
        display.count = Func_080b6c08(2, display.ids);
        display.ids[display.count] = 0x21;
        Func_080b7b6c(display.ids, 0);
        Func_080c9028(&display);
        Func_080c0cec(0, 0, 0, 100);

        *work = 0;
        Func_0800307c(2, 32, (const void *)0x080c0299);
        Func_080030f8(1);
        Func_080030f8(20);
        Func_08015128(Data_03001e74[0x41]);
        Func_080039fc((volatile void *)0x04000008, 2);
        Func_0800393c((volatile void *)0x04000008, 0);
        *(volatile u16 *)0x04000050 = 0x3f40;

        owner_count = Func_080b6c08(3, owners);
        owners[owner_count] = 0x00ff;
        Func_080b7b6c(owners, 0);
        visible_count = Func_080b6c08(1, visible);
        for (index = 0; index < visible_count; index++)
            Func_080c0f98(visible[index], 1);

        for (index = 0; index < 16; index++) {
            *(volatile u16 *)0x04000052 = index | 0x1000;
            Func_080030f8(1);
        }

        for (index = 0; index < visible_count; index++)
            Func_080c0f98(visible[index], 0);

        Func_080b595c(resource);
        *(volatile u16 *)0x03001ad2 = 0;
        Func_080030f8(1);
        Func_0800307c(2, 0, 0);
    }

    Func_0800307c(2, 0, 0);
    *(volatile u16 *)0x0400000a = 0x1f83;
    Func_080030f8(1);
    *(volatile u16 *)0x0400000a = 0x1f83;
    *(volatile u16 *)0x04000008 &= 0xfffd;
    *(volatile u16 *)0x03001ad4 = 8;
    *(volatile u16 *)0x04000000 = 0x1541;
    Func_08002dd8(42);
}
