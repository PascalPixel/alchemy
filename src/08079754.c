typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

struct State_08079754 {
    u8 padding[0x11c];
    s8 value;
};

extern struct State_08079754 Data_02000240;

s32 Func_08079754(s32 amount)
{
    s32 value;

    value = Data_02000240.value;
    value += amount;
    if (value > 28)
        value = 28;
    if (value < 0)
        value = 0;
    Data_02000240.value = (s8)value;
    return value;
}
