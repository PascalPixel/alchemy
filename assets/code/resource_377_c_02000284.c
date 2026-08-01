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

s32 Func_02001b94();
s32 Func_02001bcc(s32, s32);
s32 Func_02001be6();
s32 Func_02001c36(s32, s32, s32);
s32 Func_02001c5c(s32);
s32 Func_02001c6c(s32, s32);
s32 Func_02001c88(s32, s32);
s32 Func_02001cb2(s32, s32);

void Func_02000284(void) {
    void *temp_r2;

    Func_02001b94();
    Func_02001c36(0xE, 0, 0xA);
    Func_02001c5c(0x11AA);
    Func_02001c6c(0xE, 0);
    if (Func_02001bcc(0, 0) == 0) {
        Func_02001c88(0xE, 0);
    } else {
        temp_r2 = *(void **)0x03001EBC;
        M2C_FIELD(temp_r2, u16 *, 0x1D8) = (u16) (M2C_FIELD(temp_r2, u16 *, 0x1D8) + 1);
        Func_02001cb2(0xE, 0);
    }
    Func_02001be6();
}
