typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct DmaChannel {
    u32 source;
    u32 destination;
    u32 control;
} DmaChannel;

extern u8 Data_080f0024[];
extern u8 Value_00000230[];
struct TransferState {
    u8 unknown[0xC4];
    u32 context;
};

extern struct TransferState Data_03001e50;
extern u16 Data_02002090;
extern u16 Data_04000208;

void Func_080f0254(s32);
s32 Func_08002f40(s32);
s32 Func_080048b0(s32, s32);
void Func_080072f0(s32, s32, s32, s32);
void Func_08002dd8(s32);

void Func_080f02b0(s32 arg0, s32 arg1)
{
    s32 buffer;
    s32 destination;
    s32 palette;
    s32 paletteValue;
    s32 fill;
    register s32 size;
    s32 copy;
    u16 *count;
    u32 savedIme;
    u32 index;
    void *ime;
    u32 control;

    if (arg0 == 0) {
        Func_080f0254(arg1);
        return;
    }

    buffer = Func_08002f40(arg0);
    if (arg1 == 0) {
        fill = 0;
        paletteValue = 0x05000000;
        destination = 0x06000000;
    } else {
        fill = 0x80808080;
        paletteValue = 0x05000100;
        destination = 0x06008000;
    }
    palette = paletteValue;

    size = (s32)Value_00000230;
    copy = Func_080048b0(49, size);
    control = 0x84000000 | ((u32)size >> 2);
    *(volatile DmaChannel *)0x040000D4 = (DmaChannel) {
        (u32)Data_080f0024,
        copy,
        control,
    };

    Func_080072f0(buffer + 0x100, destination, fill,
                  Data_03001e50.context);
    Func_08002dd8(49);

    count = &Data_02002090;
    ime = &Data_04000208;
    savedIme = *(volatile u16 *)ime;
    *(volatile u16 *)ime = (u16)(u32)ime;
    index = *count;
    if ((s32)index <= 31) {
        volatile u32 *entry = (u32 *)((u8 *)count + 4 + index * 12);
        *entry++ = buffer;
        *count = index + 1;
        *entry++ = palette;
        *entry = 0x84000040;
    }
    *(volatile u16 *)ime = savedIme;
}
