typedef unsigned short u16;

/* Complete blend-alpha setter through return and its two pool words. */
void Func_020018d0(void)
{
    *(volatile u16 *)0x04000052 = 0x0607;
}
