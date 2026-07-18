typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(s32);
s32 Func_0800ca98(s32);

void Func_0800ca6c(s32 arg0) {
    s32 index;

    index = 0;
    while (index <= 0x257 && Func_0800ca98(arg0) == 0) {
        Func_080030f8(1);
        index++;
    }
}
