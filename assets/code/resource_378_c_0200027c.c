typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x0200027c (112 bytes, through 0x020002eb).
 *
 * The five literal words at 0x020002d8--0x020002eb are this owner's pool;
 * the next push, at 0x020002ec, starts the following owner.  The body has
 * ten imported calls and three conditional/unconditional branch sites.
 * The scene selector is the signed halfword at Data_02000240 + 450, built
 * as 225 * 2 by the original code.  The final two calls are deliberately
 * kept after that selector test: the second call's result is discarded by
 * the epilogue, which pops the return address into r0.
 */

extern s16 Data_02000240[];

extern void Func_02003870(void);
extern s32 Func_02003856(s32 flag);
extern void Func_02003938(s32 value);
extern void Func_02003940(s32 value);
extern void Func_02003956(s32 value);
extern void Func_02003916(s32 id, s32 enabled);
extern void Func_02003958(s32 id, s32 x, s32 y);
extern void Func_020038ae(s32 frames);
extern void Func_02003986(s32 id, s32 arg1);
extern void Func_020038ca(void);

void Func_0200027c(void)
{
    Func_02003870();

    if (Func_02003856(0x855) != 0) {
        Func_02003938(0x1377);
    } else {
        Func_02003940(0x1289);
    }

    if (Data_02000240[225] == 11) {
        Func_02003956(0x1ce9);
    }

    Func_02003916(9, 1);
    Func_02003958(9, 0, 0);
    Func_020038ae(2);
    Func_02003986(9, 0);
    Func_020038ca();
}
