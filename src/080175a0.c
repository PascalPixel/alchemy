typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(s32);
s32 Func_08017364(void);
void Func_080174f8(void);

void Func_080175a0(void) {
    Func_080174f8();
    goto check;
again:
    Func_080030f8(1);
check:
    if (Func_08017364() == 0) {
        goto again;
    }
    Func_080030f8(1);
}
