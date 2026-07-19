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

u8 Func_080a68ec(void *, void *, s32);
s32 Func_08077008(s32);
void Func_08015270(s32 *);
void Func_080a1bdc(s32, s32, s32);
void Func_080a68a8(void *);
void Func_08015080(s32, s32 *, s32, s32);

void Func_080a6384(s32 arg0) {
    void *temp_r6;
    void *temp_r7;
    s32 temp0;

    temp_r7 = *(void **)0x03001F2C;
    temp0 = Func_08077008(arg0);
    temp_r6 = (u8 *)temp_r7 + 0x1C8;
    M2C_FIELD(temp_r7, u8 *, 0x218) = Func_080a68ec((void *)temp0, temp_r6, 2);
    Func_08015270(M2C_FIELD(temp_r7, s32 *, 0x20));
    Func_080a1bdc(0x6C, 0x20, 8);
    Func_080a68a8(temp_r6);
    if (M2C_FIELD(temp_r7, u8 *, 0x218) == 0) {
        Func_08015080(0xAF2, M2C_FIELD(temp_r7, s32 *, 0x20), 0, 0x18);
    }
}
