typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern void Func_02007bfa(s32, s32, s32, s32, s32, s32);
extern void Func_02007c0a(s32, s32, s32, s32, s32, s32);
extern void Func_02007c1c(s32, s32, s32, s32, s32, s32);
extern void Func_02007c2c(s32, s32, s32, s32, s32, s32);
extern void Func_02007c48(s32, s32, s32, s32, s32, s32);

void Func_02003950(void)
{
    Func_02007bfa(78, 39, 78, 40, 5, 1);
    Func_02007c0a(78, 39, 78, 41, 5, 1);
    Func_02007c1c(78, 39, 79, 42, 4, 1);
    Func_02007c2c(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Func_02007c48(17, 38, 5, 2, x, y);
    }
}
