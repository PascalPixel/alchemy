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

extern u8 Data_000000d5[];
void Func_080030f8(u32);
s32 Func_08002f40(s32);
void Func_08005340(s32, s32);
void Func_080113e4(void);
void Func_0800439c(s32);

struct S {
    u8 pad[0x100];
    s16 a;
    s16 b;
};

void Func_0801173c(void) {
    struct S *temp_r1;

    temp_r1 = *(struct S **)0x03001E70;
    *(s32 *)0x03001CFC = 0x0801161D;
    temp_r1->a = 0;
    temp_r1->b = 0x9F;
    Func_080030f8(1U);
    Func_08005340(Func_08002f40((s32) Data_000000d5), 0x02010000);
    Func_080113e4();
    Func_0800439c(0x0801179D);
    Func_080030f8(1U);
}
