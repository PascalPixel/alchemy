typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_0802e108[];
extern u8 Data_0802de88[];

s32 Func_08019ed0(void)
{
    return (Data_0802e108 - Data_0802de88) >> 2;
}
