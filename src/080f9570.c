typedef unsigned char u8;
typedef unsigned int u32;

extern u8 Data_02003040[];

void Func_080f9570(u32 value)
{
    u32 alternate = value & 0x80;

    value &= 0x7f;
    if (alternate != 0)
        Data_02003040[0] ^= value;
    else
        Data_02003040[0] = value;
}
