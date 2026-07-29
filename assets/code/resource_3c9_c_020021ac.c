typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

typedef struct {
    u8 filler0[8];
    s32 unk08;
    u8 filler0c[4];
    s32 unk10;
} Obj;

typedef struct {
    u8 filler0[8];
    s32 unk08;
    s32 unk0c;
    s32 unk10;
    u8 filler14[4];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
    u8 filler66[2];
    Obj *unk68;
} Spr;

void Func_02007e3c(Spr *);
s32 Func_02007dec_a(s32);

void Func_020021ac(Spr *s)
{
    u16 *p = (u16 *)((u8 *)s + 0x64);
    Obj *o;
    s32 t = *p + 1;
    s16 v;

    o = s->unk68;
    *p = t;
    v = (s16)(t << 16 >> 16);
    if (v > 31) {
        Func_02007e3c(s);
    } else {
        s32 r = Func_02007dec_a(v << 10);
        s32 k;
        s->unk18 = r;
        s->unk1c = -r;
        s->unk08 = o->unk08;
        k = 0x10000;
        s->unk0c = s->unk0c + k;
        k = k - r;
        s->unk10 = o->unk10 - (k * 5) + 0x100000;
    }
}
