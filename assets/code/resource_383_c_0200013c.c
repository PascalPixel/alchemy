typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 Value_00001280;

extern u8 *Func_02004ebe(s32);
extern void Func_02004e8c(void);
extern void Func_02005066(s32, s32);
extern s32 Func_02004e76(s32);
extern void Func_02004f80(s32);
extern void Func_02004f88(s32);
extern void Func_02004fa0(s32, s32);
extern void Func_02004ecc(void);

void Func_0200013c(void)
{
    s32 value = *(u16 *)(Func_02004ebe(0) + 6);

    Func_02004e8c();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02005066(4, 19);
    } else {
        if (Func_02004e76(0x855) == 0) {
            Func_02004f80((s32)&Value_00001280);
        } else {
            Func_02004f88(0x1370);
        }
        Func_02004fa0(19, 0);
    }
    Func_02004ecc();
}
