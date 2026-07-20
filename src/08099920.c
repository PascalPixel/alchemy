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

s32 Func_08004458();
s32 Func_08009098(void *, s32);
void Func_08096bec(void *, s32, s32);
void *Func_08096c80(s32, s32, s32, s32);

void Func_08099920(void *arg0) {
    s32 var_r7;
    void *temp_r0;
    s32 phase = 2;
    s32 phase2;
    s16 *ptr2;

    if ((s32) M2C_FIELD(arg0, s32 *, 0xC) <= (s32) M2C_FIELD(arg0, s32 *, 0x14)) {
        M2C_FIELD(arg0, s16 *, 0x5E) = phase;
        Func_08009098(arg0, 0x0809F0B0);
        temp_r0 = NULL;
        M2C_FIELD(arg0, void **, 0x6C) = temp_r0;
        for (var_r7 = 0; var_r7 <= 2; var_r7++) {
            temp_r0 = Func_08096c80(0xF0, M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0xC), M2C_FIELD(arg0, s32 *, 0x10));
            if (temp_r0 == NULL) {
                break;
            }
            M2C_FIELD(temp_r0, s32 *, 0x1C) = 0x8000;
            M2C_FIELD(temp_r0, s32 *, 0x18) = 0x8000;
            M2C_FIELD(temp_r0, s8 *, 0x55) = 2;
            M2C_FIELD(temp_r0, s32 *, 0x28) = 0x10000;
            M2C_FIELD(temp_r0, s32 *, 0x30) = (s32) (Func_08004458() + 0x13333);
            Func_08096bec(temp_r0, 0x200000, Func_08004458());
            ptr2 = &M2C_FIELD(temp_r0, s16 *, 0x5E);
            phase2 = 6;
            *ptr2 = phase2;
            Func_08009098(temp_r0, 0x0809F0B0);
        }
    }
}
