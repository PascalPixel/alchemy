#include "types.h"

#define FindClearActorPosition Func_02000474
#define NoOpEffectCallback Func_02000cc0
#define RunPrologueSceneSetup Func_02001374
#define StartSceneScript37 Func_0200138c
#define RunSceneVectorTransition Func_02001740
#define TryPushBlockingSceneActor Func_02001a44
struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

extern s32 Data_0200b1b4[];
extern s32 Data_0200b15c[];
extern u8 Data_0200b2bc[];

s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_0200358c(u8 *, s32 *);
void Func_020043e8(void);
void Func_02001440(void);
void Func_020043f8(void);
void Func_02002638(void);
s32 Func_020043b6(u8 *arg0, s32 arg1, s32 arg2);
void Func_020044b4(s32 arg0);
void Func_0200447a(s32 arg0);
void Func_020047b6(void);
s32 Func_02001bc2(struct V6 *arg0);
void Func_02001d6e(struct V6 arg0);
void Func_020047e2(void);
void Func_02004a60(s32 arg0, s32 arg1, struct V *arg2);
s32 Func_02004ac0(struct S *arg0, struct V *arg1);
void Func_02004b08(void);
void Func_02004a98(struct S *arg0, s32 arg1);
void Func_02004a5e(s32 arg0);
void Func_02004bc4(s32 arg0);
void Func_02004aac(struct S *arg0, s32 arg1);
void Func_02004b0e(struct S *arg0, s32 arg1);
void Func_02004b7c(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ae4(struct S *arg0, s32 arg1);
void Func_02004b2c(struct S *arg0, s32 arg1);
void Func_02004b74(void);

s32 FindClearActorPosition(s32 *a)
{
    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Func_020007de(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Data_0200b1b4;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = Data_0200b15c[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_0200b15c[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_0200b1b4;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_0200b1b4;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_0200358c(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_0200b15c[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_0200b15c[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_0200b15c[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

/* Deliberate no-op callback. */
void NoOpEffectCallback(void) {}

void RunPrologueSceneSetup(void)
{
    Func_020043e8();
    Func_02001440();
    Func_020043f8();
    Func_02002638();
}

void StartSceneScript37(void)
{
    Func_020043b6(Data_0200b2bc, 37, 7);
    Func_020044b4(183);
    Func_0200447a(4);
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Func_020047b6();
    if (Func_02001bc2(&transition) != 0) {
        Func_02001d6e(transition);
    }
    Func_020047e2();
}

s32 TryPushBlockingSceneActor(struct S *actor)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Func_02004a60(0x200000, direction, &destination);
    }
    if (Func_02004ac0(actor, &destination) == 0) {
        s32 t;

        Func_02004b08();
        Func_02004a98(actor, 6);
        Func_02004a5e(6);
        Func_02004bc4(152);
        Func_02004aac(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Func_02004b0e(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Func_02004b7c(0, coordinates[1], coordinates[5]);
        }
        Func_02004ae4(actor, 6);
        Func_02004b2c(actor, 1);
        *state = (u8)saved_state;
        Func_02004b74();
        return 1;
    }
    return 0;
}
