typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

void Func_02003fb0(void);
void Func_020040c6(s32);
void Func_02003fe0(s32, s32, s32);
void Func_02003fea(s32, s32, s32);
u8 *Func_02003fe8(s32);
void Func_02004042(s32, s32);
void Func_0200401c(s32, s32, s32);
void Func_02004038(s32, s32, s32);
void Func_0200405e(s32);
void Func_02004076(s32, s32);
void Func_0200402a(void);

void Func_02001b44(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *q;

    Func_02003fb0();
    Func_020040c6(185);
    Func_02003fe0(arg0, 0x3333, 0x1999);
    Func_02003fea(0, 0x3333, 0x1999);
    q = Func_02003fe8(arg0) + 90;
    *q &= 0xfe;
    Func_02004042(0, 8);
    Func_0200401c(0, arg3 * 16 + 8, arg4 * 16 + 8);
    {
        s32 t1 = arg1 * 16;
        s32 t2 = arg2 * 16;
        Func_02004038(arg0, t1 + 8, t2 + 8);
    }
    Func_0200405e(arg0);
    Func_02004076(0, 1);
    Func_0200402a();
}
