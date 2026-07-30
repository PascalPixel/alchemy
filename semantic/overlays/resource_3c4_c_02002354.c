/*
 * resource_3c4 @ 0x02002354 (28 bytes: 22 code + alignment + one pool word).
 *
 * Sibling of 0x02002330 against a different service triple.  The pool word at
 * 0x0200236c is 0x00000953, again a selector rather than an address.
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_0200544c(void);
void Func_0200542c(s32 selector, s32 flag);
void Func_02005460(void);

void Func_02002354(void)
{
    Func_0200544c();
    Func_0200542c(0x953, 1);
    Func_02005460();
}
