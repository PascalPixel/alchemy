typedef signed int s32;
typedef unsigned char u8;

extern u8 *Func_02002786(s32);

void Func_02000e44(s32 arg0, s32 arg1)
{
    u8 *entry = Func_02002786(arg0);

    if (entry != 0) {
        u8 *field = entry + 0x54;

        *field = arg1;
    }
}
