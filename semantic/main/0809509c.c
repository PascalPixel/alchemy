typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

struct Record {
    u32 zero00;
    u32 config04;
    u32 config08;
    u32 zero0c;
    u32 position10;
    u32 zero14;
    u32 unused18;
    u16 frame1c;
    u16 unused1e;
};

struct State {
    volatile s32 *coordinates;
};

extern struct State * volatile Data_03001e70;
extern const void Data_080a00b8;
extern const void Data_08094e7d;
void *Func_080048f4(s32, s32);
void Func_080053e8(const void *, void *);
s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, void *);
void Func_08002dd8(s32);
s32 Func_080091a8(s32, s32, s32);
s32 Func_080041d8(const void *, s32);

void Func_0809509c(void)
{
    u32 *header = Func_080048f4(29, 0x410);
    struct Record *record = (struct Record *)(header + 2);
    struct DmaTransfer *dma = (struct DmaTransfer *)0x040000d4;
    u32 zero = 0;
    void *buffer;
    s32 handle;
    u32 index;

    dma->source = &zero;
    dma->destination = header;
    dma->control = 0x85000104;

    buffer = Func_080048f4(14, 0x400);
    Func_080053e8(&Data_080a00b8, buffer);
    handle = Func_08004080();
    header[0] = handle;
    header[1] = Func_08003fa4(handle, 0x200, buffer);
    Func_08002dd8(14);

    for (index = 0; index <= 31; index++, record++) {
        struct State *state = Data_03001e70;
        u32 *cursor = (u32 *)record;
        volatile s32 *coordinates;
        s32 x;
        s32 y;

        *cursor++ = 0;
        coordinates = state->coordinates;
        *cursor++ = 0x40000400;
        *cursor = 0xd400;
        x = coordinates[0];
        y = coordinates[2];
        record->zero0c = 0;
        record->zero14 = 0;
        record->position10 = Func_080091a8(
            0, x >> 16, y >> 16) << 16;
        record->frame1c = (index & 15) + 1;
    }

    Func_080041d8(&Data_08094e7d, 0xc80);
}
