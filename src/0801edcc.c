typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

void Func_0801edcc(void *arg0, s32 arg1)
{
    if (arg0 != 0) {
        register s32 zero asm("r3") = 0;
        ((s8 *)arg0)[5] = arg1;
        *(s16 *)((u8 *)arg0 + 12) = zero;
    }
}
