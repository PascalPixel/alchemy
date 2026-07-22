typedef unsigned char u8;
typedef unsigned int u32;

extern u8 Data_02000040[];

u32 Func_080793c8(u32 value, u8 byte)
{
    u32 shifted = value << 20;
    value = shifted >> 23;
    Data_02000040[value] = byte;
    return value;
}
