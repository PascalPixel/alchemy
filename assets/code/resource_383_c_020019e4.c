typedef signed int s32;
typedef unsigned char u8;

extern void Func_02006776(s32, s32, s32);
extern void Func_020067b4(s32, s32, s32);
extern s32 Func_0200671a(s32);
extern void Func_02006752(void);
extern void Func_02006828(s32);
extern void Func_02006840(s32, s32);
extern void Func_0200676c(void);
extern void Func_02006958(s32);
extern void Func_020068a6(s32);

void Func_020019e4(void)
{
    u8 *state;

    /* 128 << 8 and 128 << 7. */
    Func_02006776(0, 0x8000, 0x4000);
    /* 186 << 2 and 204 << 1. */
    Func_020067b4(0, 744, 408);

    if (Func_0200671a(0x854) == 0) {
        Func_02006752();
        Func_02006828(0x12C3);
        Func_02006840(8, 0);
        Func_0200676c();
    }

    state = *(u8 **) 0x03001EBC;
    *(s32 *) (state + 456) = 16;
    Func_02006958(123);
    Func_020068a6(14);
}
