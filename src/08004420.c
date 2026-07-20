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
void Func_080072e4(s32);
extern u8 Data_03001d34;
extern u8 Data_03001a20[];
void Func_08004420(s32 arg0) {
    s32 key = arg0;
    u8 *p = Data_03001a20;
    s32 i;
    key = key >> 8;
    if (Data_03001d34 == 1) {
        i = 0x15;
        p -= 8;
loop:
        i -= 1;
        if (i != 0) {
            p += 8;
            if (p[5] == key) Func_080072e4(*(s32 *)p);
            goto loop;
        }
    }
}
