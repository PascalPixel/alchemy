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

void *Func_08092054(void);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_080030f8(u32);
void Func_08009150(void *, s32, s32, s32);
void Func_08092adc(s32, s32, s32);
void Func_08092624(void *, s32);
void Func_08009158(void *);
void Func_0809202c(void);

void Func_08092708(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r3;
    s32 temp_r9;
    void *temp_r0;
    u8 *ptr55;

    temp_r0 = Func_08092054();
    temp_r9 = M2C_FIELD(temp_r0, s32, 0x10);
    if (temp_r0 != NULL) {
        Func_080f9010(0x121);
        Func_08009080(temp_r0, arg1);
        Func_080030f8(0xAU);
        ptr55 = (u8 *) temp_r0 + 0x55;
        Func_08009080(temp_r0, 1);
        { u8 v55 = 2; v55 |= *ptr55; *ptr55 = v55; }
        M2C_FIELD(temp_r0, s32, 0x28) = 0x40000;
        Func_08009150(temp_r0, M2C_FIELD(temp_r0, s32, 8), M2C_FIELD(temp_r0, s32, 0xC), temp_r9 + 0xC0000);
        Func_080030f8(6U);
        Func_080f9010(0xD9);
        arg1 = 0;
        Func_08092adc(arg0, 0x5000, 0);
        *ptr55 = 0U;
        do {
            temp_r3 = M2C_FIELD(temp_r0, s32, 0xC) + 0xFFFE0000;
            M2C_FIELD(temp_r0, s32, 0xC) = temp_r3;
            M2C_FIELD(temp_r0, s32, 0x3C) = temp_r3;
            Func_080030f8(1U);
            if ((arg2 != -1) && (1 & arg1)) {
                Func_08092624(temp_r0, arg2);
            }
            arg1 += 1;
        } while ((u32) arg1 <= 0xDU);
        *ptr55 = 3U;
        M2C_FIELD(temp_r0, s32, 0x28) = 0x30000;
        Func_08009150(temp_r0, M2C_FIELD(temp_r0, s32, 8), M2C_FIELD(temp_r0, s32, 0xC), temp_r9 + 0x100000);
        Func_08009158(temp_r0);
        arg1 = 0;
        if ((s32) M2C_FIELD(temp_r0, s32, 0xC) > (s32) M2C_FIELD(temp_r0, s32, 0x14)) {
loop_7:
            Func_080030f8(1U);
            arg1 += 1;
            if ((u32) arg1 <= 0xB3U) {
                if ((s32) M2C_FIELD(temp_r0, s32, 0xC) > (s32) M2C_FIELD(temp_r0, s32, 0x14)) {
                    goto loop_7;
                }
            }
        }
        Func_080030f8(2U);
        Func_0809202c();
    }
}
