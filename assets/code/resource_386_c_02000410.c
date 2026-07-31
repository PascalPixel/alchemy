typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_02000a7a(s32);
extern void Func_02000a58(void);
extern void Func_02000afa(s32, s32);
extern void Func_02000aca(s32);
extern void Func_02000ae2(s32, s32);
extern void Func_02000a86(void);

void Func_02000410(void)
{
    u32 place;

    place = *(u16 *) (Func_02000a7a(0) + 6);
    Func_02000a58();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000afa(6, 21);
    } else {
        Func_02000aca(0x1CE6);
        Func_02000ae2(21, 0);
    }

    Func_02000a86();
}
