typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001f3c;

void Func_020038a0(s32 a)
{
    u8 *state = Data_03001f3c;

    *(s16 *)(state + 220) = a;
}
