/*
 * resource_3c4 @ 0x02001924 (20 bytes: 14 code + alignment + one pool word).
 *
 * The pool word at 0x02001934 is 0x00000202, a small selector rather than an
 * address, so it stays an integer literal.  The second call takes no argument
 * that this body establishes.  `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_02004a06(s32 id);
void Func_02003072(void);

void Func_02001924(void)
{
    Func_02004a06(0x202);
    Func_02003072();
}
