typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

union Slot {
    s32 w;
    struct { s16 lo, hi; } h;
};

struct Obj {
    u8 pad00[6];
    u16 field06;
    union Slot u08;
    s32 field0c;
    union Slot u10;
    u8 pad14[0x22 - 0x14];
    u8 field22;
    u8 pad23[1];
    s32 field24;
    u8 pad28[4];
    s32 field2c;
    s32 field30;
    s32 field34;
    s32 field38;
    u8 pad3c[4];
    s32 field40;
    u8 pad44[0x59 - 0x44];
    u8 field59;
    u8 pad5a[0x62 - 0x5a];
    u8 field62;
};

extern u32 Data_0200ad68[];

extern struct Obj *Func_02002cf8(s32 arg0);
extern struct Obj *Func_02000176(s32 *arg0, struct Obj *arg1);
extern struct Obj *Func_020001a2(s32 *arg0, struct Obj *arg1);
extern struct Obj *Func_020001ce(s32 *arg0, struct Obj *arg1);
extern s32 Func_02002d84(struct Obj *arg0, s32 *arg1);
extern void Func_02002d5c(struct Obj *arg0, s32 arg1);
extern void Func_02002d44(s32 arg0);
extern void Func_02002eea(s32 arg0);
extern void Func_02002da2(struct Obj *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002db2(struct Obj *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002dc0(struct Obj *arg0);
extern void Func_02002f0c(void);
extern void Func_02002dc8(struct Obj *arg0, s32 arg1);

void Func_020000c4(void) {
    s32 buf[3];
    struct Obj *a;
    struct Obj *b;
    struct Obj *p;
    s32 k;
    u32 w;
    s32 c;
    s32 z;

    a = Func_02002cf8(0);
    k = a->field06 >> 12;
    w = Data_0200ad68[k];
    buf[0] = a->u08.w + (w & 0xffff0000);
    buf[1] = a->field0c;
    w <<= 16;
    buf[2] = a->u10.w + w;
    b = Func_02000176(buf, a);
    if (b == 0) return;

    w = Data_0200ad68[k];
    buf[0] = b->u08.w + (w & 0xffff0000);
    buf[1] = b->field0c;
    w <<= 16;
    buf[2] = b->u10.w + w;
    p = Func_020001a2(buf, b);
    if (p != 0 && (p->field59 & 1) != 0) return;

    buf[0] = b->u08.w;
    buf[1] = b->field0c + 0x100000;
    buf[2] = b->u10.w;
    p = Func_020001ce(buf, b);
    if (p != 0 && (p->field59 & 1) != 0) return;

    b->field22 = 2;
    w = Data_0200ad68[k];
    buf[0] = b->u08.w + (w & 0xffff0000);
    buf[1] = b->field0c;
    w <<= 16;
    buf[2] = b->u10.w + w;
    if (Func_02002d84(b, buf) > 0) return;

    z = b->field62;
    if (z != 0) return;

    Func_02002d5c(a, 8);
    c = 0x3333;
    Func_02002d44(15);
    Func_02002eea(185);
    b->field30 = c;
    b->field34 = c;
    Func_02002da2(b, buf[0], buf[1], buf[2]);
    a->field30 = c;
    a->field34 = c;
    Func_02002db2(a, buf[0], buf[1], buf[2]);
    Func_02002dc0(b);
    Func_02002f0c();
    b->u08.w = buf[0];
    b->u10.w = buf[2];
    b->field24 = z;
    b->field2c = z;
    a->field38 = 0x80000000;
    a->field40 = 0x80000000;
    a->field24 = z;
    a->field2c = z;
    a->u08.w = a->u08.h.hi << 16;
    a->u10.w = a->u10.h.hi << 16;
    Func_02002dc8(a, 1);
}
