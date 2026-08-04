#include "types.h"

struct RenderRecord_0809bb64 {
    s32 first;
    s32 second;
    s32 third;
};

struct RenderSetup_0809bb64 {
    u16 resource;
    u16 table_index;
    s32 horizontal;
    s32 vertical;
    u8 unknown_0C[6];
    u16 sentinel;
    u8 unknown_14[4];
    s32 scale;
    s32 handle;
    struct RenderRecord_0809bb64 records[67];
};

struct ReferenceObject_0809bb64 {
    u8 unknown_00[8];
    s32 position;
    u8 unknown_0C[6];
    s16 height;
};

struct ResourceSlot_0809bb64 {
    u16 first;
    u16 value;
};

struct Dma3_0809bb64 {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_08004970(s32);
void Func_08002df0(void *);
s32 Func_08003fa4(s32, s32, void *);
s32 Func_08004080(void);
s32 Func_08015010(s32, s32, s32, s32, s32);
u16 Func_080153b8(void);
s32 Func_080770c0(s32);
struct ReferenceObject_0809bb64 *Func_08092054(u32);

/*
 * Initialize the fixed render-record block at 0x02010000 and submit its
 * 0x80-byte parameter table through a newly allocated resource slot.
 */
void Func_0809bb64(void) {
    struct RenderSetup_0809bb64 *setup =
        (struct RenderSetup_0809bb64 *)0x02010000;
    struct ResourceSlot_0809bb64 *resource_slots =
        (struct ResourceSlot_0809bb64 *)0x03001B10;
    volatile struct Dma3_0809bb64 *dma =
        (volatile struct Dma3_0809bb64 *)0x040000D4;
    s32 *parameters = Func_08004970(0x20);
    struct ReferenceObject_0809bb64 *reference =
        Func_08092054(*(u32 *)0x02000434);
    s32 zero = 0;
    s32 common_value;
    s32 index;
    u16 resource_value;

    setup->resource = Func_08004080();

    dma->source = &zero;
    dma->destination = parameters;
    dma->control = 0x85000020;

    parameters[0] = 0xFF;
    parameters[1] = 0x1FF;
    parameters[8] = 0x44;
    parameters[9] = 0x144;
    parameters[16] = 0x77;
    parameters[17] = 0x177;
    parameters[24] = 0xFF0;
    parameters[25] = 0xFFFF;
    parameters[26] = 0x1FFFF;
    parameters[27] = 0x11FF0;
    parameters[2] = 0x110;
    parameters[10] = 0x110;
    parameters[18] = 0x110;
    parameters[28] = 0x1100;

    common_value =
        Func_08003fa4(setup->resource, 0x80, parameters) | 0x400;
    for (index = 0; index < 66; index++) {
        setup->records[index].first = 0;
        setup->records[index].second = 0;
        setup->records[index].third = common_value;
    }
    Func_08002df0(parameters);

    if (Func_080770c0(0x11C) != 0) {
        setup->horizontal = 0x780000;
        setup->vertical = 0x500000;
    } else {
        s32 horizontal =
            (s32)((u32)reference->position + 0xF0000000U);
        s32 vertical = reference->height * 0xA0;

        horizontal = (horizontal >> 16) * 0xF0;
        if (horizontal < 0) {
            horizontal += 0xFFF;
        }
        if (vertical < 0) {
            vertical += 0xFFF;
        }
        setup->horizontal =
            (s32)((u32)(horizontal >> 12) << 16);
        setup->vertical =
            (s32)((u32)(vertical >> 12) << 16);
    }

    setup->table_index = Func_080153b8();
    resource_value = resource_slots[setup->table_index].value;
    setup->handle = Func_08015010(0, 0, 0, 0, 2);
    setup->sentinel = 0xFFFF;
    setup->scale = 0x10000;

    setup->records[66].first = 0;
    setup->records[66].second = 0x40000000;
    setup->records[66].third = 0x400 | (resource_value >> 5);
}
