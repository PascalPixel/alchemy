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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Func_080048f4(s32, s32);
s32 Func_08009238();

void Func_0808c4c0(void) {
    void *temp_r0;

    temp_r0 = Func_080048f4(0x1B, 0xCCC);
    if (M2C_FIELD(temp_r0, s16 *, 0x19E) == 3) {
        Func_08009238();
        M2C_FIELD(M2C_FIELD(temp_r0, void **, 0x1E0), s8 *, 0x5B) = 0;
    }
}
