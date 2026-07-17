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
#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_08009080(s32, s32);
s32 Func_0808ec14();

void Func_0808ed1c(void) {
    s32 temp_r0;
    s32 temp_r0_2;

    temp_r0 = Func_0808ec14();
    if (temp_r0 != -1) {
        { s32 _mb = *(s32 *)0x03001EBC; s32 _mo = temp_r0 * 8; temp_r0_2 = *(s32 *)((s32)_mb + (s32)_mo + (s32)0x11C); }
        if (temp_r0_2 != 0) {
            Func_08009080(temp_r0_2, 2);
        }
    }
}
