typedef signed int s32;

extern s32 Func_02006a46(s32);
extern s32 Func_02006a54(s32);
extern s32 Data_0200f334[];
extern s32 Data_0200f100[];
extern s32 Data_0200ef38[];

s32 Func_02000aa4(void)
{
    if (Func_02006a46(0x87a) != 0)
        return (s32)Data_0200f334;
    if (Func_02006a54(0x815) != 0)
        return (s32)Data_0200f100;
    return (s32)Data_0200ef38;
}
