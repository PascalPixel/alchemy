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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080048f4(s32, s32);
void *Func_0808ba1c();
s32 Func_080933f8(s32, s32, s32, s32);

void Func_08093500(s32 arg0, s32 arg1) {
    void *temp_r5;

    temp_r5 = Func_0808ba1c();
    Func_080048f4(0x1B, 0xCCC);
    if (temp_r5 != NULL) {
        Func_080933f8(M2C_FIELD(temp_r5, s32 *, 8), -1, M2C_FIELD(temp_r5, s32 *, 0x10), arg1);
    }
}
