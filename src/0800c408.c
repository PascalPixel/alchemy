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

s32 Func_0800b7c0();

void Func_0800c408(void *arg0, s32 arg1) {
    if ((arg0 != NULL) && ((0xF & M2C_FIELD(arg0, u8 *, 0x54)) == 1)) {
        arg0 = M2C_FIELD(arg0, void **, 0x50);
        if (arg1 >= 0) {
            *M2C_FIELD(arg0, s16 **, 0x28) = (s16) arg1;
            Func_0800b7c0();
        }
    }
}
