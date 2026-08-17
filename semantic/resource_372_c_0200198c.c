#include "types.h"

extern void Func_0200608c(void);
extern s32 *Func_020060c2(s32);
extern void Func_02006130(s32, s32, s32);
extern void Func_020060ee(s32, s32, s32);
extern void Func_02006138(s32, s32, s32);
extern void Func_0200618a(s32, s32, s32);
extern void Func_020060c0(s32);
extern void Func_020061a6(s32);
extern void Func_020061be(s32, s32);
extern void Func_020061a0(s32, s32, s32);
extern void Func_020060de(s32);
extern void Func_020061a6_b(s32, s32);
extern void Func_020060ec(s32);
extern void Func_02006200(s32, s32, s32);
extern void Func_020061f0(s32, s32);
extern void Func_020061a8(s32, s32);
extern s32 *Func_02006146(s32);
extern void Func_02006190(s32, s32, s32);
extern void Func_020061b6(s32);
extern void Func_020061c8(s32, s32, s32);
extern void Func_020061c4(s32, s32, s32);
extern void Func_02006150(void);
void Func_0200198c(void)
{
    s32 *desc;
    s16 *pos;

    Func_0200608c();

    desc = Func_020060c2(0);
    if (desc != 0) {
        /* word fields at +8 and +16 of the descriptor */
        Func_02006130(22, desc[2], desc[4]);
    }

    Func_020060ee(22, 0x10000, 0x8000);   /* 128 << 9, 128 << 8 */
    Func_02006138(22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Func_020061a6(0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6_b(0, 1);                  /* same name, two arguments here */
    Func_020060ec(20);
    Func_02006200(22, 0x4000, 0);         /* 128 << 7 */
    Func_020061f0(22, 0);
    Func_020061a8(22, 2);

    pos = Func_02006146(0);
    if (pos != 0) {
        /* signed halfword fields at +10 and +18, as in 0x0200173c */
        Func_02006190(22, pos[5], pos[9]);
    }

    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Func_020061c4(0, 0x100, 0x205);       /* 128 << 1 */
    Func_02006150();
}
