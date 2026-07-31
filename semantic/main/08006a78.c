typedef unsigned char u8;
typedef unsigned short u16;

void Func_08006a78(void)
{
    u16 **cursor = (u16 **)0x02004C28;
    volatile u16 *interruptMasterEnable = (volatile u16 *)0x04000208;
    volatile u16 *interruptEnable = (volatile u16 *)0x04000200;
    u16 *entries;

    entries = *cursor;
    entries[0] = 0;
    *cursor = entries + 1;
    entries[1] = 0;
    *cursor = entries;

    *interruptMasterEnable = 0;
    *interruptEnable &= ~(u16)(8 << *(u8 *)0x02004C20);
    *interruptMasterEnable = *(u16 *)0x02004C2C;
}
