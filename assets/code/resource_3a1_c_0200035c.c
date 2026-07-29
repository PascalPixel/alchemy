typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

u16 *Func_020009d6(s32);
void Func_020009c4(void);
void Func_02000a2e(s32, s32);
s32 Func_020009c6(s32);
void Func_02000a20(s32);
void Func_02000a28(s32);
void Func_02000a38(s32, s32);
void Func_02000a04(void);

void Func_0200035c(void)
{
    u16 *state = Func_020009d6(0);
    u32 value = state[3];

    Func_020009c4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a2e(16, 14);
    } else {
        if (Func_020009c6(0x895) == 0) {
            Func_02000a20(0x1817);
        } else {
            Func_02000a28(0x1a46);
        }
        Func_02000a38(14, 0);
    }
    Func_02000a04();
}
