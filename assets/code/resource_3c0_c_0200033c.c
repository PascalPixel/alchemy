typedef unsigned char u8;
typedef signed int s32;

/* Complete 16-byte flag-toggle leaf, including its alignment halfword. */
s32 Func_0200033c(u8 *entity)
{
    entity[84] ^= 1;
    return 1;
}
