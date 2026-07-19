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

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_0809a484(s32, s32, s32, s32, s32, s32, s32, s32 *);

void Func_0809a6b8(void *arg0) {
    s32 sp1C[10];
    volatile u8 _pad[8];
    s32 sp10;
    s32 *p10;
    s32 *p1c;
    s32 temp_r0;
    s32 temp_r5;
    u32 temp_r3;
    u32 var_r7;

    (void)_pad;
    p1c = sp1C;
    M2C_FIELD(p1c, s32 *, 4) = 0;
    M2C_FIELD(p1c, s32 *, 0x24) = 0x0809A65D;
    M2C_FIELD(p1c, s32 *, 8) = 0xCCCC;
    M2C_FIELD(p1c, s32 *, 0xC) = 0xCCCC;
    var_r7 = 0;
    p10 = &sp10;
    do {
        temp_r5 = var_r7 << 0xC;
        temp_r3 = Func_0800231c(temp_r5) * 3;
        M2C_FIELD(p10, s32 *, 0) = (s32) ((s32) (temp_r3 + (temp_r3 >> 0x1F)) >> 1);
        M2C_FIELD(p10, s32 *, 4) = 0;
        temp_r0 = Func_08002322(temp_r5);
        M2C_FIELD(p10, s32 *, 8) = temp_r0;
        Func_0809a484(M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0xC), M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(p10, s32 *, 0), M2C_FIELD(p10, s32 *, 4), temp_r0, 0x01090001, p1c);
        var_r7 += 1;
    } while (var_r7 <= 0x10U);
}
