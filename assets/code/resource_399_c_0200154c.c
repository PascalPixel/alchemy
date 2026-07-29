typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[80];
    u16 unk64;
    s16 unk66;
} T;

extern s32 Data_0200b1f0[];
extern void Func_02003728(s32, s32, s32 *);
extern void Func_02003764(T *);

void Func_0200154c(T *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = Data_0200b1f0[0];
            b[1] = Data_0200b1f0[1] + 0x80000;
            b[2] = Data_0200b1f0[2];
            Func_02003728(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Func_02003764(o);
        }
    }
}
