typedef unsigned char u8;
typedef unsigned short u16;

void Func_020004ae();

void Func_0200015c(void)
{
    u8 *display;

    /* Start the scene, then configure alpha blending for its display state. */
    Func_020004ae(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    display = *(u8 **)0x03001ecc;
    {
        u16 *slot = (u16 *)(display + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x52a);
        int value = 10;
        *slot = value;
    }
}
