#include "types.h"

struct DmaTransfer_080f0678 {
    const void *source;
    void *destination;
    u32 control;
};

struct Command_080f0678 {
    u32 command;
    u32 value;
};

void *Func_08004970(s32);
void Func_080041d8(const void *, s32);
void Func_080f07f0(s32, s32, s32);

/*
 * Initialize the title presentation's 128-entry command table, clear and
 * seed its character VRAM, install both callbacks, and create 32 objects.
 */
void Func_080f0678(void)
{
    struct DmaTransfer_080f0678 *dma =
        (struct DmaTransfer_080f0678 *)0x040000D4;
    struct Command_080f0678 *command;
    u32 fill;
    s32 group;
    s32 item;

    command = Func_08004970(0x400);
    *(struct Command_080f0678 **)0x02004C0C = command;

    fill = 0;
    dma->source = &fill;
    dma->destination = (void *)0x06010000;
    dma->control = 0x85001800;

    fill = 0x11111111;
    dma->source = &fill;
    dma->destination = (void *)0x06016000;
    dma->control = 0x85000040;

    for (item = 0; item < 8; item++) {
        command->command = 0x80004000 | (item << 21);
        command->value = 0x300;
        command++;
    }
    for (item = 0; item < 8; item++) {
        command->command = 0x80004088 | (item << 21);
        command->value = 0x300;
        command++;
    }
    for (item = 0; item < 8; item++) {
        command->command = 0x40004098 | (item << 21);
        command->value = 0x300;
        command++;
    }

    for (group = 0; group < 16; group++) {
        u32 flags = 0x10 + group * 8;
        u32 tile = 0x180000;
        u32 value = group * 24;

        for (item = 0; item < 6; item++) {
            command->command = 0x40004000 | flags | tile;
            command->value = value;
            command++;
            tile += 0x200000;
            value += 4;
        }
    }

    for (item = 0; item < 8; item++) {
        command->command = 0x00C000C0;
        command->value = 0x300;
        command++;
    }

    *(u16 *)0x02004C00 = 0;
    *(u16 *)0x02004C08 = 0;
    *(u16 *)0x02004C04 = 0;
    Func_080041d8((const void *)0x080F0539, 0x480);
    Func_080041d8((const void *)0x080F0615, 0xC80);

    for (item = 0; item < 32; item++) {
        Func_080f07f0(*(s32 *)0x080F1220, item * 0x18, 1);
    }
}
