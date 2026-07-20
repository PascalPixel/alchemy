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

s32 Func_080b6b40(s32, s16 *);
void *Func_08077008(s32);
s32 Func_080022ec(s32, s32);
s32 Func_08004458(void);
extern u8 Data_02000240[];

s32 Func_080b90f8(void) {
    s32 sp4;
    u8 *sp0;
    s16 sp8[14];
    s32 count;
    s32 total;
    s32 i;
    s32 accum;
    void *temp_r2;

    sp4 = 0;
    temp_r2 = *(void **)0x03001E74;
    if (M2C_FIELD(temp_r2, u8 *, 0x45) == 1) {
        sp4 = 1;
    } else {
        sp0 = (u8 *)temp_r2 + 0x46;
        accum = 0x1388 + (M2C_FIELD(temp_r2, u8 *, 0x46) * 0x7D0);
        count = Func_080b6b40(1, sp8);
        total = 0;
        for (i = sp4; i < count; i++) {
            total += M2C_FIELD(Func_08077008((s32) sp8[i]), u8 *, 0xF);
        }
        accum += Func_080022ec(total * 0x1F4, count);
        count = Func_080b6b40(2, sp8);
        total = 0;
        for (i = 0; i < count; i++) {
            total += M2C_FIELD(Func_08077008((s32) sp8[i]), u8 *, 0xF);
        }
        accum -= Func_080022ec(total * 0x1F4, count);
        if ((accum > 0) && ((u32) ((u32) (0x2710 * Func_08004458()) >> 0x10) < (u32) accum)) {
            sp4 = 1;
        }
        *sp0 += 1;
    }
    if (Data_02000240[0x22B] == 2) {
        sp4 = 0;
    }
    return sp4;
}
