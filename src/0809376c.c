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

s16 *Func_0808d394(s16);
void *Func_08185000(s16);

s32 Func_0809376c(void *arg0) {
    void *temp_r6;

    temp_r6 = M2C_FIELD(arg0, void **, 0x68);
    if (temp_r6 != NULL) {
        M2C_FIELD(arg0, s8 *, 0x55) = 0;
        M2C_FIELD(arg0, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(temp_r6, s32 *, 0xC) + (M2C_FIELD(Func_08185000(*Func_0808d394(M2C_FIELD(arg0, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(temp_r6, s32 *, 0x14);
        M2C_FIELD(arg0, s32 *, 0x10) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
    }
    return 0;
}
