typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_0200059a(s32);
extern void Func_02000580(void);
extern void Func_02000602(s32, s32);
extern s32 Func_02000582(s32);
extern void Func_020005dc(s32);
extern void Func_020005e4(s32);
extern void Func_020005fc(s32, s32);
extern void Func_020005c0(void);

void Func_0200005c(void)
{
    u32 place;

    place = *(u16 *) (Func_0200059a(0) + 6);
    Func_02000580();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000602(7, 16);
    } else {
        if (Func_02000582(0x845) == 0) {
            Func_020005dc(0x13E3);
        } else {
            Func_020005e4(0x16F5);
        }
        Func_020005fc(16, 0);
    }

    Func_020005c0();
}
