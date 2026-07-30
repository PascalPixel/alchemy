typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_02001804(s32);

void Func_020001f0(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter = (s16 *)(state + 0x16C);
    s32 value = *counter;

    *(s32 *)(state + 0x1C8) = 16;
    Func_02001804(value);
}
