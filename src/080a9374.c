typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001f2c;
void *Func_08077008(s32);
void Func_080a345c(void);
void Func_080a68a8(void *);

void Func_080a9374(s32 unused, s32 value)
{
    u8 *base = Data_03001f2c;

    Func_08077008(value);
    Func_080a345c();
    Func_080a68a8(base + 456);
}

void Func_080a939c(void)
{
}
