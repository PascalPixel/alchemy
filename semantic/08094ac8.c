#include "types.h"

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

struct Source {
    s32 first;
    s32 unknown_04;
    s32 second;
};

struct Record {
    u32 unknown_00;
    u32 attributes;
    u32 tile;
    s32 first;
    s32 transformed;
    s32 second;
    u32 unknown_18;
    u16 sequence;
    u16 unknown_1e;
};

struct Buffer {
    void *first;
    void *second;
    struct Record records[32];
};

void *Func_080048f4(s32 arena, s32 size);
void Func_08091ff0(s32 value);
void Func_080053e8(const void *source, void *destination);
void *Func_08004080(void);
void *Func_08003fa4(void *value, s32 size, void *memory);
void Func_08002dd8(s32 arena);
s32 Func_080091a8(s32 mode, s32 first, s32 second);
void Func_080041d8(const void *callback, s32 value);

void Func_08094ac8(void)
{
    struct Buffer *buffer = Func_080048f4(0x1D, 0x410);
    struct Source *source = **(struct Source ***)0x03001E70;
    void *memory;
    struct Record *record;
    s32 zero;
    u32 index;

    Func_08091ff0(0xAA);
    record = buffer->records;
    zero = 0;
    *(const void **)0x040000D4 = &zero;
    *(void **)0x040000D8 = buffer;
    *(u32 *)0x040000DC = 0x85000104;

    memory = Func_080048f4(0x0E, 0x400);
    Func_080053e8((const void *)0x0809FF58, memory);
    buffer->first = Func_08004080();
    buffer->second = Func_08003fa4(buffer->first, 0x300, memory);
    Func_08002dd8(0x0E);

    for (index = 0; index <= 31; index++, record++) {
        s32 first;
        s32 second;
        s32 record_zero = 0;

        record->unknown_00 = record_zero;
        record->attributes = 0x40000400;
        record->tile = 0xD400;
        first = source->first;
        second = source->second;
        record->first = first;
        record->second = second;
        record->transformed = Func_080091a8(
            0, first >> 16, second >> 16) << 16;
        record->sequence = (index & 0x0F) + 1;
    }

    *(volatile u16 *)0x04000050 = 0x3F00;
    *(volatile u16 *)0x04000052 = 0x1008;
    *(volatile u16 *)0x04000054 = 0;
    Func_080041d8((const void *)0x08094821, 0xC80);
}
