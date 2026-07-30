#include "types.h"

struct DeferredWrite_080cdbc0 {
    u32 value;
    u32 address;
    u32 mask;
};

void Func_080f9010(s32 sound);
void Func_080072f0(s32 source, s32 size, s32 register_address, s32 work);
void Func_08004278(void *callback);
void Func_080030f8(u32 frames);
void Func_080b5038(s32 mode, u16 object, s32 arg2);
void Func_080b5048(u16 object, s32 position);

static void QueueDisplayControl_080cdbc0(u32 value)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    volatile u16 *count = (volatile u16 *)0x02002090;
    u16 saved = *interrupt_master;

    *interrupt_master = (u16)(u32)interrupt_master;
    if (*count <= 31) {
        struct DeferredWrite_080cdbc0 *write =
            (struct DeferredWrite_080cdbc0 *)
                (0x02002090 + *count * sizeof(*write));

        (*count)++;
        write->value = value;
        write->address = 0x04000000;
        write->mask = 0x00020000;
    }
    *interrupt_master = saved;
}

void Func_080cdbc0(void)
{
    u8 *scene = *(u8 **)0x03001eec;
    u8 *runtime = *(u8 **)0x03001e74;
    u16 object = *(u16 *)(runtime + 0x648);
    s32 index;

    Func_080f9010(0x121);
    *(s16 *)0x03001ad4 = *(s32 *)(scene + 0x77a0);
    *(s16 *)0x03001ad6 = *(s32 *)(scene + 0x77a4);
    *(s32 *)0x03001cec = 0x78;
    *(s32 *)0x03001cf0 = 0x78;
    *(volatile u16 *)0x0400000c = 0x0787;
    Func_080072f0(0x06004000, 0x4000, 0x0400000c, 0x03000164);
    Func_08004278((void *)0x080cd4b5);
    *(s16 *)0x03001ad6 = 0x20;

    QueueDisplayControl_080cdbc0(0x7341);
    *(volatile u16 *)0x04000050 = 0;
    Func_080030f8(1);
    Func_080b5038(2, object, 7);
    Func_080030f8(1);

    for (index = 0; index < 8; index++) {
        Func_080b5048(object, 0x15 - index * 3);
        Func_080030f8(1);
    }

    QueueDisplayControl_080cdbc0(0x7541);
    Func_080030f8(1);
}
