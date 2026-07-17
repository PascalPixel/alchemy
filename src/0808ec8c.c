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

s32 Func_080030f8(s32);
s32 Func_08009080(void *, s32);
s32 Func_080091e0(void *, s32);
s32 Func_0808ec14();
s32 Func_080f9010(s32);

void Func_0808ec8c(void) {
    s32 temp_r0;
    void *temp_r5;

    temp_r0 = Func_0808ec14();
    if (temp_r0 != -1) {
        { s32 _mb = *(s32 *)0x03001EBC; s32 _mo = temp_r0 * 8; temp_r5 = *(void **)((s32)_mb + (s32)_mo + (s32)0x11C); }
        Func_080030f8(0x12);
        if (temp_r5 != NULL) {
            Func_08009080(temp_r5, 7);
        }
        Func_080f9010(0x92);
        if (temp_r5 != NULL) {
            M2C_FIELD(temp_r5, s32 *, 0x28) = 0x80000;
            Func_080091e0(temp_r5, 1);
        }
    }
}
