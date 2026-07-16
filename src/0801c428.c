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

s32 Func_08004278(s32);
s32 Func_08016418(void *, s32);

void Func_0801c428(void) {
    void *temp_r0;

    temp_r0 = M2C_FIELD(*(void **)0x03001EBC, void **, 0x230);
    if ((temp_r0 != NULL) && (M2C_FIELD(temp_r0, u16 *, 0x16) != 0)) {
        Func_08016418(temp_r0, 2);
        Func_08004278(0x0801C3E9);
    }
}
