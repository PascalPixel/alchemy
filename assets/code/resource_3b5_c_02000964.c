typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_020017aa(s32);

void Func_02000964(void)
{
    s16 *counter = (s16 *)(Data_03001ebc + 0x16C);

    Func_020017aa(*counter);
}
