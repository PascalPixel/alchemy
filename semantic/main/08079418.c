typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
extern u8 Data_02000040[];

s32 Func_08079418(u32 value, u32 shift)
{
    u32 shifted = value << 20;
    s32 result;
    shift = value & 4;
    value = shifted >> 23;
    result = Data_02000040[value];
    result &= 15 << shift;
    return result >> shift;
}
