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
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08096cdc(s32, s32, s32);

void Func_0809728c(void)
{
    s32 _v1d26;
    s32 _v1live6;
    s32 _v1live5;
    s32 _v1d6;
    s32 _v1d23;
    s32 temp_r7;
    s32 temp_r5;
    void *temp_r6;
    void *temp_sl;

    temp_r6 = *(void **)0x03001F30;
    temp_sl = *(void **)0x03001EBC;
    _v1d26 = 0;
    _v1live6 = (s32)(*(s32 *)((u8 *)(temp_r6) + 0x10));
    temp_r5 = _v1live6;
    _v1live5 = (s32)(*(s16 *)((u8 *)(temp_r6) + 0x1C));
    temp_r7 = _v1live5;
    Func_08009080(temp_r5, 0x14);
    M2C_FIELD(temp_r5, u32 *, 0x38) = (s32)*(s32 *)((u8 *)(temp_r5) + 8);
    M2C_FIELD(temp_r5, s32 *, 0x3C) = (s32)M2C_FIELD(temp_r5, s32 *, 0xC);
    M2C_FIELD(temp_r5, s32 *, 0x40) = (s32)M2C_FIELD(temp_r5, s32 *, 0x10);
    M2C_FIELD(temp_r5, u32 *, 0x24) = 0;
    M2C_FIELD(temp_r5, u32 *, 0x28) = 0;
    *(s32 *)((u8 *)(temp_r5) + 0x2C) = 0;
    if ((s8)M2C_FIELD(temp_r6, s8 *, 0x22) != 0) {
        Func_080f9010(212);
        M2C_FIELD(temp_r5, s32 *, 0x6C) = 0x08096F15;
    }
    if ((s8)M2C_FIELD(temp_r6, s8 *, 0x23) != 0) {
        Func_08096cdc(temp_r5, 1, 0);
        Func_08015120((s32)temp_r7, 4);
        if ((s8)M2C_FIELD(temp_r6, s8 *, 0x21) != 0) {
            Func_08015040((void *)0x926, (s32)*(s8 *)((u8 *)(temp_r6) + 0x71C));
        } else {
            Func_08015040((void *)0x926, (s32)*(s8 *)((u8 *)(temp_r6) + 0x71C));
        }
        Func_08096cdc(temp_r5, 0, 0x10);
    }
    if (Func_080770c0(0x140) != 0) {
        if ((s8)M2C_FIELD(temp_r6, s8 *, 0x22) != 0) {
            M2C_FIELD(temp_r5, s32 *, 0x6C) = 0x08096F51;
        }
        Func_08009080(temp_r5, 0x15);
    } else {
        Func_08097174();
    }
    M2C_FIELD(temp_sl, s8 *, 0xCC7) = 1;
}
