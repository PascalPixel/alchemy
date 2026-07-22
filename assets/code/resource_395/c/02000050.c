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

void Func_020019f0();
void Func_02001a14();
void Func_02001ae0(s16);
void Func_02001b0a(s32, s32, s32);

void Func_02000050(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001EBC;
    Func_020019f0();
    Func_02001b0a(0, 0, 0);
    Func_02001ae0(M2C_FIELD(temp_r5, s16 *, 0x16C));
    Func_02001a14();
}
