typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

void Func_02000560();
void Func_0200051e();

s32 Func_02000200(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *workspace;
    u8 *display;
    s32 offset = 224;
    s32 *scene;

    /* Reset the scene word at workspace + 448 before entering scene 9. */
    offset <<= 1;
    workspace = *base;
    scene = (s32 *)(workspace + offset);
    offset -= 192;
    *scene = offset;

    Func_02000560(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    display = base[4];
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

    Func_0200051e();
    return 0;
}
