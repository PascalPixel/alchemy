typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Entry_08011b54 {
    u32 source;
    u16 field4;
    u16 field6;
    u16 value8;
    u16 count;
    u8 destination[32];
};

struct State_08011b54 {
    struct Entry_08011b54 entries[4];
    u16 used;
};

extern struct State_08011b54 *Data_03001ec0;

s32 Func_08011b54(s16 row, s16 column, s16 value, s16 count)
{
    struct State_08011b54 *state = Data_03001ec0;
    struct Entry_08011b54 *entry;
    u32 source;
    u32 destination;
    u32 control;
    volatile u32 *dma;

    if (state->used > 3)
        return -1;

    entry = &state->entries[state->used];
    source = 0x05000000 + (((u16)row * 16 + (u16)column) * 2);
    entry->field4 = 0;
    entry->field6 = 0;
    entry->count = count;
    entry->source = source;
    entry->value8 = value;

    destination = (u32)entry->destination;
    control = (u16)count | 0x80000000;
    dma = (volatile u32 *)0x040000D4;
    *dma++ = source;
    *dma++ = destination;
    *dma++ = control;

    state->used++;
    return 0;
}
