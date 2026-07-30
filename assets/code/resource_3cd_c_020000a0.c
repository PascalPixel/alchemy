typedef signed int s32;
typedef unsigned char u8;

extern u8 *Func_020008f8(s32);
extern void Func_02000932(s32, s32);
extern void Func_02000910(s32);

void Func_020000a0(s32 arg0, s32 arg1)
{
    u8 *entry = Func_020008f8(arg0);

    Func_02000932(arg0, entry[15] + arg1);
    Func_02000910(arg0);
}
