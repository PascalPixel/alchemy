/*
 * resource_3c4 @ 0x02001938 (56 bytes: 50 code + alignment + one pool word).
 *
 * The slot-21 counterpart of 0x0200170c.  The pool word at 0x0200196c is
 * 0x00000201, a selector rather than an address.
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

void Func_02004a84(s32 slot, s32 arg1);
void Func_02004aa4(s32 slot, s32 arg1);
void Func_02004a94(s32 slot, s32 arg1);
u8 *Func_02004a62(s32 slot);
void Func_02004a42(s32 selector);

void Func_02001938(void)
{
    u8 *descriptor;

    Func_02004a84(21, 1);
    Func_02004aa4(21, 0);
    Func_02004a94(21, 2);
    descriptor = Func_02004a62(21);
    descriptor[35] &= (u8)0xfd;
    Func_02004a42(0x201);
}
