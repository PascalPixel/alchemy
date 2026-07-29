typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_0200091a(s32);
extern void Func_02000954(s32);
extern void Func_0200093c(s32);
extern void Func_0200094c(s32, s32);

void Func_02000454(void)
{
    T *p;

    p = Func_0200091a(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_02000954(21);
    } else {
        Func_0200093c(0x266b);
        Func_0200094c(22, 0);
    }
}
