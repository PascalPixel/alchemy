typedef signed int s32;
typedef unsigned char u8;

extern void Func_020006d4(u8 *);

u8 *Func_02000080(void)
{
    u8 *slot = (u8 *)0x020087F4;

    Func_020006d4(slot);
    return slot;
}
