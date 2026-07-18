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

void Func_080049ac(void);
s32 Func_080770c0(s32);
s32 Func_080051e8(void *, void *);
s32 Func_080051d8(void *, void *);
void Func_080072f0(void *);

s32 Func_080b7ed8(void) {
    u8 *state = *(u8 **)0x03001E80;
    Func_080049ac();
    if (Func_080770c0(0x16B) != 0) {
        ((void (*)(void *))0x030002C0)((void *)0x080C2A7C);
        return Func_080051e8(state, state + 0xC);
    } else {
        return Func_080051d8(state, state + 0xC);
    }
}
