typedef signed int s32;
typedef unsigned short u16;

extern u16 Data_03001b10[];
s32 Func_08003fa4(s32, u16, s32);

s32 Func_080040d0(s32 index, s32 value)
{
    return Func_08003fa4(index, Data_03001b10[index * 2], value);
}
