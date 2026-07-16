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

void Func_08092064(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s32 *, 0x34) = arg2;
        M2C_FIELD(temp_r0, s32 *, 0x30) = arg1;
    }
}
