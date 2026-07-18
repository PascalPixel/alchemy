typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(s32);
s32 Func_08017364(void);
s32 Func_08017658(s32, s32, s32, s32);

s32 Func_08021e48(s32 arg0, s32 arg1, s32 arg2) {
    s32 result;

    result = Func_08017658(arg0, arg1, arg2, 1);
    goto check;
again:
    Func_080030f8(1);
check:
    if (Func_08017364() == 0) {
        goto again;
    }
    return result;
}
