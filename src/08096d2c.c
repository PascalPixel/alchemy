typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08096d2c(void *arg0) {
    s32 temp_r0;
    s32 temp_r3;
    void *temp_r6;

    temp_r3 = M2C_FIELD(arg0, u16 *, 0x64) + 1;
    temp_r6 = *(void * volatile *)((u8 *)arg0 + 0x68);
    *(volatile s16 *)((u8 *)arg0 + 0x64) = temp_r3;
    temp_r0 = (s16)temp_r3;
    if (temp_r0 > 0x1F) {
        Func_08009098((s32)arg0, 0x0809F0B0);
        return;
    }
    temp_r0 = Func_08002322(temp_r0 << 10);
    M2C_FIELD(arg0, s32 *, 0x18) = temp_r0;
    M2C_FIELD(arg0, s32 *, 0x1C) = temp_r0;
    M2C_FIELD(arg0, s32 *, 8) = M2C_FIELD(temp_r6, s32 *, 8);
    M2C_FIELD(arg0, s32 *, 0xC) += 0x10000;
    M2C_FIELD(arg0, s32 *, 0x10) =
        M2C_FIELD(temp_r6, s32 *, 0x10) + ((0x10000 - temp_r0) * 5) + 0x90000;
}
