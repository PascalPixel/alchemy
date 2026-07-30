typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

s32 *Func_02003dce(s32);
void Func_02003dc6(void);
void Func_02003edc(s32);
void Func_02003df6(s32, s32, s32);
void Func_02003e00(s32, s32, s32);
u8 *Func_02003dfe(s32);
void Func_02003e50(s32, s32);
u8 *Func_02003e16(s32);
void Func_02003e32(s32, s32, s32);
void Func_02003e3e(s32, s32, s32);
void Func_02003e64(s32);
void Func_02003e7c(s32, s32);
void Func_02003e30(void);

void Func_0200195c(void) {
    s32 *p = Func_02003dce(17);
    u8 *q;
    u8 *r;

    if (p == NULL || (p[4] >> 20) != 8) {
        return;
    }
    Func_02003dc6();
    Func_02003edc(185);
    Func_02003df6(17, 0x3333, 0x1999);
    Func_02003e00(0, 0x3333, 0x1999);
    q = Func_02003dfe(17) + 90;
    *q &= 0xfe;
    Func_02003e50(0, 8);
    r = Func_02003e16(0);
    Func_02003e32(0, *(s16 *)(r + 10), 136);
    Func_02003e3e(17, 288, 120);
    Func_02003e64(17);
    Func_02003e7c(0, 1);
    Func_02003e30();
}
