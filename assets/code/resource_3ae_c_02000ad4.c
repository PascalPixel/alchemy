typedef signed int s32;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_02002048(s32);

void Func_02000ad4(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    Func_02002048(0x12F);
}
