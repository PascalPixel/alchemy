typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct DmaRegisters {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32, s32);
void Func_080053e8(s32, void *);
s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, s32);
u8 Func_08002dd8(s32);
s32 Func_080091a8(s32, s32, s32);
void Func_080041d8(s32, s32);

void Func_08094da0(void)
{
    s32 zero;
    s32 first;
    s32 second;
    u32 index;
    u8 *header;
    u8 *records;
    void *scratch;
    void *source;
    volatile struct DmaRegisters *dma;
    volatile u16 *display;

    header = Func_080048f4(0x1D, 0x410);
    zero = 0;
    records = header + 8;
    dma = (volatile struct DmaRegisters *)0x040000D4;
    dma->source = &zero;
    dma->destination = header;
    dma->control = 0x85000104;

    scratch = Func_080048f4(0xE, 0x400);
    Func_080053e8(0x080A001E, scratch);
    first = Func_08004080();
    FIELD(header, s32, 0) = first;
    FIELD(header, s32, 4) = Func_08003fa4(first, 0x300, (s32)scratch);
    Func_08002dd8(0xE);

    index = 0;
    do {
        u32 *initial = (u32 *)records;

        *initial++ = 0;
        source = **(void ***)0x03001E70;
        *initial++ = 0x40000400;
        *initial = 0xD400;
        first = FIELD(source, s32, 0);
        second = FIELD(source, s32, 8);
        FIELD(records, s32, 0xC) = first;
        FIELD(records, s32, 0x14) = second;
        FIELD(records, s32, 0x10) = Func_080091a8(0, first >> 16, second >> 16) << 16;
        FIELD(records, s16, 0x1C) = (index & 0xF) + 1;
        index++;
        records += 0x20;
    } while (index <= 0x1F);

    display = (volatile u16 *)0x04000050;
    *display++ = 0xFC << 6;
    *display++ = 0x1008;
    *display = 0;
    Func_080041d8(0x08094BBD, 0xC80);
}
