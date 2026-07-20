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

s32 Func_0807882c(void *, s32);
s32 Func_08079cbc(void *);
s32 Func_080022ec(s32, s32);
s32 Func_08079bc4(void);

u16 Func_08079d1c(void *arg0) {
    void *r6;
    s32 r5;

    if (M2C_FIELD(arg0, u8, 0x129) == 0) {
        return 1;
    }
    r6 = (void *)Func_0807882c(arg0, 1);
    if (r6 == NULL) {
        return 1;
    }
    if (M2C_FIELD(r6, u16, 0xE) == 0) {
        return 1;
    }
    r5 = Func_080022ec((Func_08079cbc(arg0) + (M2C_FIELD(r6, u8, 0xB) * 5)) << 0x10, 100);
    if (r5 > (s32) (Func_08079bc4() & 0xFFFF)) {
        return M2C_FIELD(r6, u16, 0xE);
    }
    return 1;
}
