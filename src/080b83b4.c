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

void *Func_080b7dd0(s32);
void Func_080b83b0(void *, s32);

void Func_080b83b4(s32 arg0, s32 arg1) {
    void *temp_r0;
    void *obj1;
    void *obj2;
    s32 a1;
    s32 a2;
    s32 b1;
    s32 b2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    obj1 = M2C_FIELD(Func_080b7dd0(arg0), void *, 0);
    temp_r0 = Func_080b7dd0(arg1);
    obj2 = M2C_FIELD(temp_r0, void *, 0);

    a1 = M2C_FIELD(obj1, s32, 56);
    if (a1 == (s32)0x80000000) {
        a1 = M2C_FIELD(obj1, s32, 8);
    }
    b1 = M2C_FIELD(obj1, s32, 64);
    if (b1 == (s32)0x80000000) {
        b1 = M2C_FIELD(obj1, s32, 16);
    }
    a2 = M2C_FIELD(obj2, s32, 56);
    if (a2 == (s32)0x80000000) {
        a2 = M2C_FIELD(obj2, s32, 8);
    }
    b2 = M2C_FIELD(obj2, s32, 64);
    if (b2 == (s32)0x80000000) {
        b2 = M2C_FIELD(obj2, s32, 16);
    }

    pos.x = (a1 + a2) / 2;
    pos.y = 0;
    pos.z = (b1 + b2) / 2;

    Func_080b83b0(&pos, 0x1000);
}
