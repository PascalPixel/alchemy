typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define NULL ((void *)0)

void *Func_08015e8c(void);
void Func_08003f3c(u32);
s32 Func_08016584(void *, void *);

struct TableEntry {
    u16 unused;
    u16 value;
};

struct Input {
    u8 pad[12];
    u16 x;
    u16 y;
};

struct Output {
    s32 zero;
    s8 one4;
    s8 one5;
    s16 x;
    s16 y;
    u8 padA[4];
    s8 index;
    u8 sentinel;
    u8 pad10[4];
    s32 packed;
    u32 table;
};

s32 Func_0801eadc(s32 arg0, s32 arg1, struct Input *arg2, s32 arg3, s32 arg4)
{
    s32 x;
    struct Output *output;
    s32 y;

    output = Func_08015e8c();
    if (output == NULL) {
        Func_08003f3c((u32)arg0);
        return 0;
    }
    x = arg3 + (arg2->x * 8) + 8;
    y = arg4 + (arg2->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    output->packed = (x << 16) | y | arg1;
    output->table = ((struct TableEntry *)0x03001b10)[arg0].value >> 5;
    output->sentinel = 0xff;
    output->zero = 0;
    output->x = x;
    output->y = (s16)y;
    output->index = (s8)arg0;
    output->one4 = 1;
    output->one5 = 1;
    Func_08016584(arg2, output);
    return (s32)output;
}
