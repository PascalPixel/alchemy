typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08004458(void);

void Func_080e155c(s32 arg0, u32 arg1) {
    s32 temp_r3;
    s32 temp_r6_2;
    s32 *temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r5;
    s32 temp_r6;
    void *temp_r1_2;
    void *temp_r1_3;
    void *temp_r2_2;
    void *temp_r2_3;

    temp_r1 = *(s32 *)0x03001EEC;
    temp_r0 = (s32 *)(temp_r1 + 0x77A8);
    if (*temp_r0 > 0) {
        temp_r6 = (arg0 - 1) & Func_08004458();
        temp_r5 = (s32)(arg1 + (arg1 >> 0x1F)) >> 1;
        temp_r2 = ((arg1 - 1) & Func_08004458()) - temp_r5;
        temp_r1_2 = (void *)0x03001AD0;
        temp_r6_2 = temp_r6 - temp_r5;
        temp_r3 = temp_r2 + 0x20;
        M2C_FIELD(temp_r1_2, s16 *, 4) = temp_r6_2;
        M2C_FIELD(temp_r1_2, s16 *, 6) = temp_r3;
        temp_r1_3 = (void *)0x03001CE0;
        M2C_FIELD(temp_r1_3, s32 *, 0xC) = 0x78 - temp_r6_2;
        M2C_FIELD(temp_r1_3, s32 *, 0x10) = 0x78 - temp_r2;
        *temp_r0 -= 1;
        return;
    }
    temp_r0_2 = M2C_FIELD(temp_r1, s32 *, 0x77A0);
    temp_r2_2 = (void *)0x03001AD0;
    M2C_FIELD(temp_r2_2, s16 *, 4) = temp_r0_2;
    temp_r0_2 = M2C_FIELD(temp_r1, s32 *, 0x77A4);
    M2C_FIELD(temp_r2_2, s16 *, 6) = temp_r0_2;
    temp_r2_3 = (void *)0x03001CE0;
    M2C_FIELD(temp_r2_3, s32 *, 0xC) = 0x78;
    M2C_FIELD(temp_r2_3, s32 *, 0x10) = 0x78;
}
