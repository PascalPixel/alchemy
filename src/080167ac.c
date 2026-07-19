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

void Func_080167ac(void *arg0) {
    u32 _v1t37;
    s32 _v1t34;
    s32 _v1d43;
    u32 _v1t33;
    u32 _v1d60;
    void *temp_r2;

    temp_r2 = *(void **)0x03001E8C;
    _v1t33 = (s32) (M2C_FIELD(arg0, u16 *, 0x16)); M2C_FIELD(temp_r2, u16 *, 0xEAE) = (u16) _v1t33;
    _v1t34 = (s32) (M2C_FIELD(arg0, u16 *, 0x18)); (s32) _v1d60 = 0; M2C_FIELD(temp_r2, u16 *, 0xEAC) = (u16) _v1t34;
    _v1t37 = (M2C_FIELD(arg0, u16 *, 0x1A)); (s32) _v1d43 = 0; M2C_FIELD(temp_r2, u16 *, 0xEA8) = (u16) _v1t37;
}
