typedef unsigned char u8;
typedef signed int s32;

struct State_08079728 {
    u8 padding[0x118];
    s32 value;
};

extern struct State_08079728 Data_02000240;

s32 Func_08079728(s32 amount)
{
    s32 value;

    value = Data_02000240.value;
    value += amount;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    Data_02000240.value = value;
    return value;
}
