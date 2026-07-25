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

s32 Func_02000118(s32);
void Func_0200012a(s32, s32, s32);
void Func_02000132(s32);
void Func_0200013e(void);

s32 Func_02000054(void) {
    void *temp_r1;
    /* The three scale values are locals, not literals at the call, so that
       their materialisation sits in the entry block instead of the call's.
       Local CSE only unifies identical large constants inside one basic
       block; folding these back into the argument list collapses the three
       movs/lsls pairs to one pair plus two register copies. */
    s32 sx = 0x10000;
    s32 sy = 0x10000;
    s32 sz = 0x10000;

    temp_r1 = *(void **)0x03001EBC;
    M2C_FIELD(temp_r1, s32 *, 0x1C0) = 0x204;
    M2C_FIELD(temp_r1, s32 *, 0x1C8) = 0x10;
    if (Func_02000118(0x814) != 0) {
        Func_02000132(0x8D);
        Func_0200012a(sx, sy, sz);
        Func_0200013e();
    }
    return 0;
}
