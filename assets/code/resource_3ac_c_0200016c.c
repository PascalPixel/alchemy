typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern T *Func_02000632(s32);
extern void Func_0200066c(s32);
extern s32 Func_02000624(s32);
extern void Func_02000644(void);
extern void Func_02000662(s32);
extern void Func_02000672(s32, s32);
extern void Func_0200065e(void);
extern void Func_0200065c(void);
extern void Func_0200067a(s32);
extern void Func_0200068a(s32, s32);
extern void Func_02000676(void);

void Func_0200016c(void)
{
    T *p;

    p = Func_02000632(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_0200066c(21);
    } else if (Func_02000624(0x941) != 0) {
        Func_02000644();
        Func_02000662(0x2507);
        Func_02000672(21, 0);
        Func_0200065e();
    } else {
        Func_0200065c();
        Func_0200067a(0x1bdc);
        Func_0200068a(21, 0);
        Func_02000676();
    }
}
