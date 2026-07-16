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

void Func_08017620(s32 arg0) {
    void *temp_r2;

    temp_r2 = *(void **)0x03001E8C;
    if (temp_r2 != NULL) {
        if (arg0 & 1) {
            M2C_FIELD(temp_r2, s8 *, 0x12FA) = 1;
        }
        if (2 & arg0) {
            M2C_FIELD(temp_r2, s8 *, 0x12FB) = 1;
        }
    }
}
