typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 Data_02009814;
extern T *Func_020017d4(s32);

s32 Func_020003f8(void)
{
    s32 *d;
    T *p;

    d = &Data_02009814;
    p = Func_020017d4(3);
    *d = p->unk12;
    return 0;
}
