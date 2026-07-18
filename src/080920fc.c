typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)

u8 *Func_0808ba1c(s32);
void Func_08093a6c(u8 *, s32);
void Func_08009148(u8 *);

void Func_080920fc(s32 arg0, s32 arg1) {
    u8 *object = Func_0808ba1c(arg0);
    if (object != NULL) {
        s32 value = 1;
        value |= object[0x5A];
        object[0x5A] = value;
        Func_08093a6c(object, arg1);
        Func_08009148(object);
    }
}
