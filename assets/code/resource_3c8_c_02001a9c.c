typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    u8 filler0[8];
    s32 unk8;
} Struct_1a9c;

extern void Func_02006934(void);
extern Struct_1a9c *Func_02006952(s32);
extern void Func_02002400(void);
extern void Func_020034ce(Struct_1a9c *);
extern void Func_02003512(Struct_1a9c *);
extern Struct_1a9c *Func_02006974(s32);
extern void Func_02002422(void);
extern void Func_020034f0(Struct_1a9c *);
extern void Func_02003534(Struct_1a9c *);
extern void Func_02006984(void);

void Func_02001a9c(void)
{
    Struct_1a9c *o;

    Func_02006934();
    o = Func_02006952(11);
    if (o->unk8 >> 20 == 8) {
        Func_02002400();
        Func_020034ce(o);
    } else {
        Func_02003512(o);
    }
    o = Func_02006974(12);
    if (o->unk8 >> 20 == 7) {
        Func_02002422();
        Func_020034f0(o);
    } else {
        Func_02003534(o);
    }
    Func_02006984();
}
