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

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad1[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} T;

extern T *Func_080048b0(s32, s32);
extern s32 Func_0801a5a4(T *, s32);
extern s32 Func_08004080(void);
extern s32 Func_08003fa4(s32, s32, u8 *);
extern s32 Func_08002dd8(s32);
extern s32 Data_08029a10[];
extern s32 Data_08029e00[];

void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4) {
    T *t;

    t = Func_080048b0(0x11, 0x608);
    t->f604 = Data_08029a10[arg1];
    t->f600 = 2;
    t->f602 = 2;
    Func_0801a5a4(t, 0);
    t->f604 = Data_08029e00[arg0];
    t->f600 = 2;
    t->f602 = 2;
    Func_0801a5a4(t, 1);
    if (arg4 == 0) {
        *arg2 = Func_08004080();
    }
    *arg3 = Func_08003fa4(*arg2, 0x80, &t->f400);
    Func_08002dd8(0x11);
}
