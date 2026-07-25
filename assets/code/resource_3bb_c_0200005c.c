typedef unsigned char u8;
typedef signed int s32;

u8 Func_02003f0e(s32, s32, s32, s32, s32, s32);
u8 Func_02003f2a(s32, s32, s32, s32, s32, s32);
u8 Func_02003f62(s32);
u8 Func_02003f86(s32);
void *Func_02003fb4(s32);

void Func_0200005c(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = Func_02003fb4(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        Func_02003f62(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f0e(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        Func_02003f86(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f2a(0x22, 0x4D, 1, 1, v0, v1);
    }
}
