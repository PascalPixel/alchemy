typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

extern u8 *Data_03001ebc;

Obj *Func_0200709a(s32);
s32 Func_02007064(s32);
void Func_02002920(void);

void Func_02001a44(void)
{
    Obj *o = Func_0200709a(0);

    if (Func_02007064(856) == 0) {
        s32 ax = o->x / 0x100000;
        s32 az = o->z / 0x100000;

        if (ax == 16 && az > 55 && az <= 58
            && (o->dir == 0xc000 || o->dir == 0x4000)) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 40;

            *q = v;
            Func_02002920();
        }
    }
}
