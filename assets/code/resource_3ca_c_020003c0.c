typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 Data_0200980c;
extern T *Func_0200179c(s32);

s32 Func_020003c0(void)
{
    s32 *d;
    T *p;

    d = &Data_0200980c;
    p = Func_0200179c(0);
    *d = p->unk12;
    return 0;
}
