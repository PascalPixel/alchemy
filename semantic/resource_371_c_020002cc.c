typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* Copy the halfword at workspace + 282 into the object held at arg0 + 80,
 * then clear the object's byte at +38. */
s32 Func_020002cc(void *arg0)
{
    u8 *workspace = *(u8 **)0x03001e70;
    u8 *object = *(u8 **)((u8 *)arg0 + 80);
    u16 value = *(u16 *)(workspace + 282);

    *(u16 *)(object + 30) = value;
    object[38] = 0;
    return 1;
}
