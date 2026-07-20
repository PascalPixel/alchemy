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

extern s32 Func_08004080(void);
extern s32 Func_08003fa4(s32, s32, u8 *);
extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

void Func_080ae88c(void) {
    s32 temp_r0;
    s32 temp_r0_2;
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    temp_r0 = Func_08004080();
    M2C_FIELD(temp_r5, s16, 0x392) = (s16) temp_r0;
    if (temp_r0 != -1) {
        Func_08003fa4(temp_r0, 0x80, Data_080aed4c);
    }
    temp_r0_2 = Func_08004080();
    M2C_FIELD(temp_r5, s16, 0x394) = (s16) temp_r0_2;
    if (temp_r0_2 != -1) {
        Func_08003fa4(temp_r0_2, 0x80, Data_080aedcc);
    }
}
