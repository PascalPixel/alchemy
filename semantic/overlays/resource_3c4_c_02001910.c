/*
 * resource_3c4 @ 0x02001910 (20 bytes: 14 code + alignment + one pool word).
 *
 * The pool word at 0x02001920 is 0x0200b3b8, a RAM data block, so it is
 * spelled as a symbol rather than a literal.  `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_0200b3b8[];

void Func_02004a34(s32 id, void *table);

void Func_02001910(void)
{
    Func_02004a34(19, Data_0200b3b8);
}
