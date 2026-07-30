typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj;

typedef struct {
    u8 filler0[0x50];
    Obj *obj;
} Spr;

Spr *Func_0200621c(s32);
s32 Func_02000976(u8 *, u8 *);

s32 Func_020004bc(void)
{
    u8 *st = *(u8 **)0x03001ebc;
    Spr **p;
    s32 best = 0;
    Spr *m;
    s32 limit;
    u32 i;

    limit = 640;
    m = Func_0200621c(0);
    i = 8;
    p = (Spr **)(st + 0x34);
    do {
        Spr *s = *p++;
        if (s != 0) {
            if (*s->obj->unk28 == 0xf2) {
                s32 d = Func_02000976((u8 *)m + 8, (u8 *)s + 8);
                if (d < limit) {
                    limit = d;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}
