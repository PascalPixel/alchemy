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

void *Func_0808ba1c();
s32 Func_080f9010(s32);

void Func_08092560(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r0;
    register u32 old_value asm("r3");
    register u32 new_value asm("r2");
    register u8 *byte_ptr asm("r1");
    register s32 shifted asm("r3");

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        byte_ptr = (u8 *)temp_r0 + 0x55;
        old_value = *byte_ptr;
        new_value = 2;
        new_value |= old_value;
        shifted = arg1 << 0x10;
        *byte_ptr = new_value;
        M2C_FIELD(temp_r0, s32 *, 0x28) = shifted;
        if (arg1 > 5) {
            Func_080f9010(0x99);
        } else {
            Func_080f9010(0x98);
        }
        Func_0809163c(arg2);
    }
}
