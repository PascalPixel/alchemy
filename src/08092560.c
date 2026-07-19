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

void Func_0809163c(s32 arg0);
s32 Func_0808ba1c(u32 arg0);
void Func_080f9010(s32 arg0);

void Func_08092560(u32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;
    u8 *ptr;
    s32 val;
    s32 shifted;

    temp_r0 = Func_0808ba1c(arg0);
    if (temp_r0 != 0) {
        ptr = (u8 *) (temp_r0 + 85);
        val = *ptr | 2;
        shifted = arg1 << 0x10;
        *ptr = (u8) val;
        M2C_FIELD(temp_r0, s32 *, 0x28) = shifted;
        if (arg1 > 5) {
            Func_080f9010(0x99);
        } else {
            Func_080f9010(0x98);
        }
        Func_0809163c(arg2);
    }
}
