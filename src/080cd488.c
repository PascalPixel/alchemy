typedef unsigned char u8;
typedef unsigned int u32;

void Func_080cd488(void)
{
    u8 *base = *(u8 **)0x03001EEC;
    *(u32 *)0x04000028 = *(u32 *)(base + 0x77D0);
    *(u32 *)0x0400002C = *(u32 *)(base + 0x77D4);
}
