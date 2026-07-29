typedef signed short s16;
typedef signed int s32;

extern s16 Data_02001000;

extern void Func_02006ab8(s32);

void Func_02002fa0(void)
{
    s16 *p = &Data_02001000;

    while (*p != 9) {
        Func_02006ab8(1);
    }
}
