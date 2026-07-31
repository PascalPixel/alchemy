typedef signed int s32;

extern void Func_02001d3e(s32, s32, s32, s32, s32, s32);
extern void Func_02001e88(s32, s32, s32);
extern void Func_02001dee(s32, s32, s32);

void Func_02000870(void)
{
    s32 fifth = 21;
    s32 sixth = 9;

    Func_02001d3e(85, 9, 1, 1, fifth, sixth);
    Func_02001e88(100, 0, 0);
    Func_02001dee(14, 0x1580000, 0x980000);
}
