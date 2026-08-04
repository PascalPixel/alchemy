typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* Raise the pending bit on the input record, then refresh the halfword at
 * +30 of its object from the shared workspace entry at +282. */
s32 Func_020002f0(void *arg0)
{
    u8 *record = arg0;
    u8 *workspace = *(u8 **)0x03001e70;
    u8 *object = *(u8 **)(record + 80);
    u8 orred = (u8)(record[89] | 1);

    record[89] = orred;
    *(u16 *)(object + 30) = *(u16 *)(workspace + 282);
    return 1;
}
