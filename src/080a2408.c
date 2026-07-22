typedef unsigned char u8;

extern u8 * volatile Data_03001e8c;

void Func_080a2408(void)
{
    u8 *base = Data_03001e8c;
    u8 *field = base + 0xEA6;
    u8 value = 1;
    *field = value;
}

void Func_080a2420(void)
{
    u8 *base = Data_03001e8c;
    u8 *field = base + 0xEA6;
    u8 value = 0;
    *field = value;
}
