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

s32 Func_080030f8(s32);
s32 Func_080042c8(s32);
s32 Func_0808a330(s32, s32);
s32 Func_0808a348(s32);

void Func_0800c5b4(void) {
    Func_080042c8(0x0800C62D);
    Func_080042c8(0x0800C881);
    Func_0808a330(0x10000, 1);
    Func_0808a348(1);
    Func_080030f8(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}
