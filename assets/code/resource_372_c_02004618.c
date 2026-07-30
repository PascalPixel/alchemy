typedef signed int s32;
typedef unsigned char u8;
extern u8 Value_00001120;

extern void Func_02008d18(void);
extern void Func_02008cf8(s32, s32);
extern void Func_02008d2c(void);

void Func_02004618(void)
{
    Func_02008d18();
    Func_02008cf8((s32)&Value_00001120, 1);
    Func_02008d2c();
}
