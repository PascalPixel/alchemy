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

void Func_080030f8(s32);
void Func_080a1114(void *, s32);
void Func_080a195c();
void Func_080a345c();

void Func_080a34c0(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_080a195c();
    Func_080a345c();
    Func_080030f8(1);
    M2C_FIELD(M2C_FIELD(temp_r5, void **, 0x17C), s8 *, 5) = 0xD;
    Func_080a1114(temp_r5 + 0x10, 1);
    Func_080a1114(temp_r5 + 0x20, 1);
    Func_080a1114(temp_r5 + 0x10C, 1);
    Func_080a1114(temp_r5 + 0x24, 1);
    Func_080a1114(temp_r5 + 0x28, 1);
    Func_080a1114(temp_r5 + 0x2C, 1);
    Func_080a1114(temp_r5 + 0x30, 1);
    Func_080a1114(temp_r5 + 0x34, 1);
    Func_080a1114(temp_r5 + 0x38, 1);
    Func_080a1114(temp_r5 + 0x3C, 1);
    Func_080a1114(temp_r5 + 0x40, 1);
}
