typedef unsigned char u8;
typedef unsigned int u32;

extern u8 Data_02000040[];

u32 Func_08079390(u32 value)
{
    u32 shifted = value << 20;
    u32 index = shifted >> 23;
    u32 bit = value & 7;
    u32 mask = 1 << bit;
    u32 result;

    Data_02000040[index] ^= mask;
    result = Data_02000040[index] & mask;
    return (result | -result) >> 31;
}
