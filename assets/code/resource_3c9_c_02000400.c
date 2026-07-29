typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj;

typedef struct {
    u8 filler0[0x50];
    Obj *obj;
} Spr;

void Func_02000400(Spr *s)
{
    Obj *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfa10f601;
            o->unk1e = t;
        }
    }
}
