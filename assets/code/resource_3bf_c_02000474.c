typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

typedef struct Req {
    s32 kind;       /* 0x00 */
    s32 f4;         /* 0x04 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    s32 f14;        /* 0x14 */
} Req;

extern s32 Data_0200df30[];
extern s32 Data_0200ded8[];

Obj *Func_020007de(s32 *, s32 *, Req *);
s32 Func_02005b3c(Obj *, s32 *);

s32 Func_02000474(Req *s)
{
    s32 out;
    s32 vec[3];
    Obj *r;
    s32 n;
    s32 nx, ny;
    u8 *flag;
    s32 i, j;
    s32 w1, w2;
    s32 off;

    s->f14 = 0;
    r = Func_020007de(&out, &s->f4, s);
    if (r == 0) {
        return 0;
    }
    flag = (u8 *)r + 0x22;
    *flag = 2;
    {
        s32 k = s->kind;

        n = 0;
        off = k * 4;
        w1 = Data_0200df30[off + 1];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 3];
        if (w2 < 0) {
            w2 = -w2;
        }
        ny = (w1 + w2) >> 4;
        w1 = Data_0200df30[off];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 2];
        if (w2 < 0) {
            w2 = -w2;
        }
        nx = (w1 + w2) >> 4;
    }
    vec[0] = r->x + (Data_0200ded8[out] & 0xffff0000);
    {
        s32 ry = r->y;

        vec[1] = ry;
        vec[2] = r->z + (Data_0200ded8[out] << 16);
        s->y = ry;
    }
    for (;;) {
        s->z = vec[2] + (Data_0200df30[s->kind * 4 + 1] << 16);
        for (j = 0; j < ny; j++) {
            s->x = vec[0] + (Data_0200df30[s->kind * 4] << 16);
            for (i = 0; i < nx; i++) {
                if (Func_02005b3c(r, &s->x) == 2) {
                    goto found;
                }
                s->x += 0x100000;
            }
            s->z += 0x100000;
        }
        n++;
        vec[0] += Data_0200ded8[out] & 0xffff0000;
        vec[2] += Data_0200ded8[out] << 16;
    }
found:
    *flag = 0;
    if (n == 0) {
        return 0;
    }
    s->x = r->x + (Data_0200ded8[out] & 0xffff0000) * n;
    s->y = r->y;
    s->z = r->z + (Data_0200ded8[out] << 16) * n;
    return 1;
}
