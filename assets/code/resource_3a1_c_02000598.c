typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02000678(s32, s32, s32, s32);
void Func_02000686(s32, s32, s32, s32);
void Func_02000c3e(s32);
void Func_02000c76(s32, s32);

s32 Func_02000598(void)
{
    s16 scene;

    *(s32 *)(Data_03001ebc + 448) = 521;
    scene = Data_02000240[225];
    if (scene == 4 || scene == 7) {
        Func_02000686(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        Func_02000678(0x01cc0000, 0, 0x02380000, 20);
        Func_02000686(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        Func_02000c3e(0x12f);
        Func_02000c76(10, 6);
    }
    return 0;
}
