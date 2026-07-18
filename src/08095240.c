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

u8 *Func_080048f4(s32, s32);

struct Fields_08095240 {
    u8 filler[0x1F80];
    u16 field1F80;
    u16 field1F82;
};

void Func_08095240(void) {
    struct Fields_08095240 *state = (struct Fields_08095240 *)Func_080048f4(30, 0x1F88);
    state->field1F80 = 12;
    state->field1F82 = 0;
}
