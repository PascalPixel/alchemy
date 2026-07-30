typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_02000e2a(s32);
extern void Func_02000e68(s32);

void Func_0200021c(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter = (s16 *)(state + 0x16C);

    Func_02000e2a(*counter);
    Func_02000e68(0x7B);
}
