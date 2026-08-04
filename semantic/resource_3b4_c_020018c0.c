typedef unsigned short u16;

/* Complete blend-alpha setter through return, alignment and pool word. */
void Func_020018c0(void)
{
    *(volatile u16 *)0x04000052 = 0x0d00;
}
