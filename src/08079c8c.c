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

void *Func_08077394();
void *Func_0807882c(void *, s32);
s32 Func_080798b4();

s32 Func_08079c8c(void) {
    void *temp_r0;
    void *temp_r0_2;

    temp_r0_2 = Func_08077394();
    if (M2C_FIELD(temp_r0_2, u8 *, 0x129) == 0) {
        return Func_080798b4();
    }
    temp_r0 = Func_0807882c(temp_r0_2, 1);
    if (temp_r0 != NULL) {
        return M2C_FIELD(temp_r0, s32 *, 0x14);
    }
    return 4;
}
