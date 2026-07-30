/*
 * resource_3c4 @ 0x02002330 (36 bytes: 30 code + alignment + one pool word).
 *
 * The pool word at 0x02002350 is 0x0000268b, too small to be an address in
 * this image, so it is kept as an integer selector.
 *
 * 0x0200233e, the third call onwards, is a secondary entry into this owner
 * used by Func_02001530.  It is not a separate function: it shares this
 * owner's `push {lr}` frame and epilogue.  It is named in the neighbouring
 * source only as a declaration; the code lives here.
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_02005428(void);
void Func_02005480(s32 arg0, s32 arg1);
void Func_02005410(s32 selector, s32 flag);
void Func_02005444(void);

void Func_02002330(void)
{
    Func_02005428();
    Func_02005480(0, 1);
    /* secondary entry 0x0200233e */
    Func_02005410(0x268b, 1);
    Func_02005444();
}
