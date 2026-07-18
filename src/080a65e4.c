typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_080a65e4 {
    u8 padding[0x220];
    u16 values[2];
};

extern struct State_080a65e4 Data_02000240;

s32 Func_080a65e4(s32 arg0, s32 arg1, s32 arg2)
{
    s32 masked = arg1 & 0x3fff;
    s32 value = (arg0 << 10) | masked;

    if (arg2 == 0) {
        Data_02000240.values[0] = value;
    } else {
        Data_02000240.values[1] = value;
    }
    return 1;
}
