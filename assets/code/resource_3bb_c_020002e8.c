typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];

extern void Func_020042aa(s32, s32, s32);

void Func_020002e8(void)
{
    s16 *table = Data_02000240;

    Func_020042aa(*(s32 *)(table + 250), 6, 0);
}
