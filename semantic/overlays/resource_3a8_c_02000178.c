typedef unsigned char u8;

/*
 * Complete eight-byte leaf at 0x02000178: a PC-relative load of the sole pool
 * word at 0x0200017c followed by `bx lr`.  The word is the in-image data
 * address 0x0200c6b8, so the owner returns that data block's address.
 */
extern u8 Data_0200c6b8[];

u8 *Func_02000178(void)
{
    return Data_0200c6b8;
}
