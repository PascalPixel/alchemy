typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern s32 Data_03001e8c;

u8 *Func_08017dd4(u8 *, s32, s32);
void Func_0801de5c(u16 *, s32, s32, s32);

struct Command_0801ea3c {
    u16 words[8];
};

void Func_0801ea3c(s32 arg0, void *arg1, u32 arg2,
                   u32 arg3, s32 arg4) {
    u8 converted[16];
    struct Command_0801ea3c command;
    u8 *source;
    u16 *destination;
    s32 count;
    s32 mapBase;
    u32 vertical;
    u32 horizontal;
    u32 horizontal_shift;
    register u32 index;
    s32 byteOffset;

    mapBase = Data_03001e8c;
    source = Func_08017dd4(converted, arg0, 4);
    if (arg4 == 0)
        command.words[0] = 0xF01D;
    else
        command.words[0] = 0xF01F;
    command.words[1] = 0xF01E;
    destination = command.words;
    destination += 2;
    count = 4;
    do {
        count--;
        *destination++ = *source++;
    } while (count >= 0);
    command.words[6] = 0;

    vertical = FIELD(arg1, u16, 0xE);
    vertical += arg3 >> 3;
    horizontal_shift = arg2 >> 3;
    horizontal = FIELD(arg1, u16, 0xC);
    horizontal += horizontal_shift;
    vertical++;
    vertical <<= 5;
    vertical += horizontal;
    index = vertical + 1;
    if (index < 0x280U) {
        byteOffset = index * 2;
        Func_0801de5c(command.words,
                      byteOffset + mapBase,
                      byteOffset + 0x06002000,
                      arg2 & 7);
    }
}
