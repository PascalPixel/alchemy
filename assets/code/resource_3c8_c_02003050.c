typedef signed int s32;
typedef unsigned int u32;

extern void Func_02007f06(s32);

void Func_02003050(void)
{
    u32 i;

    i = 15;
    do {
        Func_02007f06(i);
        i++;
    } while (i <= 18);
}
