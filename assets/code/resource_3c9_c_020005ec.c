typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[0x3d];
    u8 unk55;
} Spr;

extern Spr *Data_0200e6e8;

Spr *Func_0200633e(s32);
void Func_0200632c(void);
Spr *Func_02000ab8(void);
void Func_02006320(s32);
Spr *Func_0200635e(Spr *);
void Func_0200623e(s32);
void Func_02006258(s32);
void Func_02006274(s32);
void Func_0200628c(s32);
void Func_020063e6(void);

void Func_020005ec(void)
{
    Spr *a = Func_0200633e(0);
    Spr *b;
    Spr *r;
    s32 k;

    Func_0200632c();
    r = Func_02000ab8();
    Data_0200e6e8 = r;
    if (r != 0) {
        Func_02006320(592);
        b = Func_0200635e(Data_0200e6e8);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Func_0200623e(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Func_02006258(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Func_02006274(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Func_0200628c(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Func_020063e6();
}
