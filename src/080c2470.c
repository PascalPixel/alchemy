typedef unsigned char u8;
typedef unsigned int u32;

u8 *Func_08077018(u32);

u32 Func_080c2470(u32 value)
{
    u32 bank = 0;
    u32 base = value & 0x1ff;
    if (base == 0)
        return 0;
    {
        u8 flags = Func_08077018(base)[3];

        if (flags & 8)
            bank = 1;
        bank <<= 1;
        if (flags & 4)
            bank++;
        bank <<= 9;
        bank += base;
    }
    return bank;
}
