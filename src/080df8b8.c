typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void **Func_080b5098(s32);
void Func_08009140(void *);
void Func_08009150(void *, s32, s32, s32);
void Func_08009080(void *, s32);

void Func_080df8b8(s32 arg0) {
    void **context = Func_080b5098(arg0);
    u8 *object = *context;

    *(s32 *)(object + 52) = 0x20000;
    *(s32 *)(object + 48) = 0x80000;
    *(s32 *)(object + 40) = 0x40000;
    *(s32 *)(object + 72) = 0xAB85;
    *(s32 *)(object + 68) = 0;
    object[90] = 0;
    object[88] = 1;
    Func_08009140(object);
    Func_08009150(object, (s32)context[3], 0, (s32)context[4]);
    Func_08009080(object, 1);
}
