typedef unsigned char u8;
typedef signed int s32;

u8 *Func_08077080(s32);
void Func_08019fcc(s32, s32, s32, s32, s32);

void Func_08019f98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Func_08019fcc(Func_08077080(arg0)[4], arg1, arg2, arg3, arg4);
}
