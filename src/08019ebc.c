typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_0802a2e0[];
extern u8 Data_08029ee4[];

s32 Func_08019ebc(void)
{
    return (Data_0802a2e0 - Data_08029ee4) >> 2;
}
