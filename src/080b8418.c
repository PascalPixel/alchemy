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

s32 Func_080030f8(s32);
void *Func_08009048(s32, s32);
s32 Func_08009070(void *, s32);
void **Func_080b7dd0();

void Func_080b8418(void) {
    void *temp_r0;
    void *temp_r0_2;

    temp_r0 = *Func_080b7dd0();
    if ((temp_r0 != NULL) && ((0xF & M2C_FIELD(temp_r0, u8 *, 0x54)) == 1)) {
        temp_r0_2 = Func_08009048(M2C_FIELD(temp_r0, s32 *, 0x50), 0x11B);
        if (temp_r0_2 != NULL) {
            Func_08009070(temp_r0_2, 1);
            M2C_FIELD(temp_r0_2, s8 *, 6) = 3;
        }
        Func_080030f8(0xA);
    }
}
