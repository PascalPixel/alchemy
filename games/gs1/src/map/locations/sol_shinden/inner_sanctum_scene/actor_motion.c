#include "types.h"

#define UpdateStatueTrapActor Func_02000c00
#define UpdateStatueLight1 Func_02000d9c
#define UpdateStatueLight2 Func_02000ef8
#define UpdateStatueLight3 Func_0200101c
#define UpdateStatueLight4 Func_02001140
typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} EntA;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern u8 Value_00001000;
extern u16 Data_0200ade4;
/* 手番カウンタ。他のオーバーレイからも書き換わるため volatile。
 * The reference reloads this cell on the path where the compiler can prove the
 * value is unchanged, which in ordinary C only a volatile object produces. */
extern volatile s32 Data_0200ade8;
extern u16 Data_0200addc;
extern u16 Data_0200ade0;
extern u16 Data_0200adec;

EntA *Func_020035f2(s32);
s32 Func_020035c2(s32);
s32 Func_020035ce(s32);
void Func_02000d2a(void);
s32 Func_020035de(s32);
s32 Func_02003608_a();
s32 Func_02003608_b();
void Func_02003660(s32, s32);
void Func_02003696(s32);
s32 Func_020035fe(s32);
EntA *Func_0200364a(s32);
void Func_02003688(s32, s32, s32);
s32 Func_0200363e_a();
s32 Func_0200363e_b();
void Func_02003674(s32, s32, s32);
void Func_020036c2(s32, s32, s32);
void Func_02003678(s32);
void Func_020036ae(s32, s32, s32);
s32 Func_02003676(s32);
void Func_020036e8(s32, s32, s32);
void Func_02003698(s32);
void Func_020036fc(s32, s32, s32);
void Func_02003770(s32, s32, s32);
void Func_0200377c_a();
void Func_0200377c_b();
void Func_0200377e(s32, s32, s32);
void Func_02003746(s32, s32);
s32 Func_020036d6(s32);
s32 Func_020036e0(s32);
void Func_0200375c(s32, s32);
EntB *Func_0200372a(s32);
void Func_0200374c(s32, s32, s32);
void Func_0200376a(s32);
void Func_02003778(s32, s32, s32);
void Func_02003786(s32, s32, s32);
void Func_0200375a(void);
s32 Func_02003726(void);
void Func_020038ca(s32);
void Func_0200377e_a(s32, s32, s32, s32, s32, s32);
void Func_02003790(s32, s32, s32, s32, s32, s32);
void Func_02003808(s32, s32, s32, s32, s32, s32);
void Func_020037b0(s32, s32, s32, s32, s32, s32);
void Func_020037d0(s32, s32, s32, s32, s32, s32);
void Func_020037f6(s32, s32, s32, s32, s32, s32);
void Func_0200381e(s32, s32, s32, s32, s32, s32);
s32 Func_0200381a(void);
void Func_02003870(s32, s32, s32);
void Func_02003890(s32, s32, s32);
s32 Func_02003882(void);
void Func_02003a26(s32);
void Func_020038da(s32, s32, s32, s32, s32, s32);
void Func_020038ec(s32, s32, s32, s32, s32, s32);
void Func_02003964(s32, s32, s32, s32, s32, s32);
void Func_0200390c(s32, s32, s32, s32, s32, s32);
void Func_0200392c(s32, s32, s32, s32, s32, s32);
void Func_02003952(s32, s32, s32, s32, s32, s32);
void Func_0200397a(s32, s32, s32, s32, s32, s32);
s32 Func_02003976(void);
s32 Func_020039a6(void);
void Func_02003b4a(s32);
void Func_020039fe(s32, s32, s32, s32, s32, s32);
void Func_02003a10(s32, s32, s32, s32, s32, s32);
void Func_02003a88(s32, s32, s32, s32, s32, s32);
void Func_02003a30(s32, s32, s32, s32, s32, s32);
void Func_02003a50(s32, s32, s32, s32, s32, s32);
void Func_02003a76(s32, s32, s32, s32, s32, s32);
void Func_02003a9e(s32, s32, s32, s32, s32, s32);
s32 Func_02003a9a(void);
s32 Func_02003aca(void);
void Func_02003c6e(s32);
void Func_02003b22(s32, s32, s32, s32, s32, s32);
void Func_02003b34(s32, s32, s32, s32, s32, s32);
void Func_02003bac(s32, s32, s32, s32, s32, s32);
void Func_02003b54(s32, s32, s32, s32, s32, s32);
void Func_02003b74(s32, s32, s32, s32, s32, s32);
void Func_02003b9a(s32, s32, s32, s32, s32, s32);
void Func_02003bc2(s32, s32, s32, s32, s32, s32);
s32 Func_02003bbe(void);

void UpdateStatueTrapActor(void)
{
    EntA *scene_actor;
    EntA *target_actor;
    EntB *target_position;
    s32 g1 = 0x810;
    s32 g2 = 0x810;
    s32 g3 = 0x810;
    s32 g4 = 0x810;
    s32 s1 = 0x10000;
    s32 s2 = 0x8000;
    s32 s3 = 0x20000;
    s32 s4 = 0x10000;
    s32 s5 = 0x4000;
    s32 d1 = 0x120;
    s32 d2 = 0x120;
    s32 d3 = 0x120;
    s32 d4 = 0x120;
    s32 d5 = 0xc000;
    s32 h1;
    s32 h2;

    scene_actor = Func_020035f2(16);
    if (Func_020035c2(0x809) == 0) {
        return;
    }
    if (Func_020035ce(0x814) != 0) {
        Func_02000d2a();
        return;
    }
    if (Func_020035de(0x819) != 0) {
        return;
    }
    Func_02003608_a();
    Func_02003660(0, 0);
    Func_02003696((s32)&Value_00001000);
    if (Func_020035fe(g1)!= 0 || Func_02003608_b(0x80a) == 0) {
        target_actor = Func_0200364a(0);
        if (target_actor != 0) {
            Func_02003688(16, target_actor->unk8, target_actor->unk10);
        }
        Func_0200363e_a(4);
        Func_02003674(16, s1, s2);
    } else {
        if (Func_0200363e_b(g2)!= 0) goto do1;
        h1 = 0x1540000;
        if (scene_actor->unk8 > h1) {
do1:
            Func_020036c2(16, 0x1880000, 0xa80000);
            Func_02003678(4);
            Func_020036ae(16, s3, s4);
        }
    }
    if (Func_02003676(g3)!= 0) goto do2;
    h2 = 0x1540000;
    if (scene_actor->unk8 > h2) {
do2:
        Func_020036e8(16, d1, 0xe8);
    } else {
        Func_02003698(0x80a);
    }
    Func_020036fc(16, d2, 0xe8);
    Func_02003770(0, d5, 0);
    Func_0200377c_a(16, s5, 10);
    Func_0200377e(16, 0, 10);
    Func_02003746(0, 3);
    if (Func_020036d6(g4)!= 0 || Func_020036e0(0x80a) == 0) {
        Func_0200375c(16, 2);
        target_position = Func_0200372a(0);
        if (target_position != 0) {
            Func_0200374c(16, target_position->unkA, target_position->unk12);
        }
        Func_0200376a(16);
        Func_0200377c_b(16, 0, 0);
        Func_02003778(0, d3, 0xe8);
    } else {
        Func_02003786(0, d4, 0xf8);
    }
    Func_0200375a();
}

void UpdateStatueLight1(void)
{
    volatile s32 *st;
    s32 s;
    s32 t1 = 0x10000;
    s32 t2 = 0x10000;
    s32 t3 = 0x10000;
    s32 m1 = -1;
    s32 m2 = -1;
    s32 m3 = 0xe666;

    if ((Func_02003726() & 3) != 0) {
        u16 v = Data_0200ade4;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_020038ca(0xbb);
            a = 1;
            b = 5;
            Func_0200377e_a(0x2e, 0x3b, 30, 0x21, a, b);
            break;
        }
        case 1:
            Func_02003790(0x2e, 0x3b, 30, 0x21, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_020037b0(0x2e, 0x3b, 30, 0x22, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_020037d0(0x2e, 0x3b, 30, 0x23, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            volatile s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_020037f6(0x2e, 0x3b, 30, 0x24, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x25, v, 5);
            break;
        case 0x50: {
            s32 a = 1;
            s32 b = 10;
            Func_0200381e(0x2e, 0x31, 30, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade4;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_0200381a() * 40) >> 16) + 90) {
                *c = 0;
            }
        }
    }
    st = &Data_0200ade8;
    s = *st;
    if (s != 0) {
        if (s == 2) {
            Func_02003870(t1, t2, t3);
        } else if (s == 1) {
            Func_02003890(m1, m2, m3);
        }
        *st = *st - 1;
    }
}

void UpdateStatueLight2(void)
{
    if ((Func_02003882() & 3) != 0) {
        u16 v = Data_0200addc;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003a26(0xbb);
            a = 1;
            b = 5;
            Func_020038da(0x2f, 0x3b, 42, 0x21, a, b);
            break;
        }
        case 1:
            Func_020038ec(0x2f, 0x3b, 42, 0x21, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_0200390c(0x2f, 0x3b, 42, 0x22, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_0200392c(0x2f, 0x3b, 42, 0x23, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003952(0x2f, 0x3b, 42, 0x24, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x25, v, 5);
            break;
        case 0x5a: {
            s32 a = 1;
            s32 b = 10;
            Func_0200397a(0x2f, 0x31, 42, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200addc;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003976() * 40) >> 16) + 100) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Func_020039a6() & 3) != 0) {
        u16 v = Data_0200ade0;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003b4a(0xbb);
            a = 1;
            b = 5;
            Func_020039fe(0x30, 0x3b, 31, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003a10(0x30, 0x3b, 31, 0x24, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003a30(0x30, 0x3b, 31, 0x25, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003a50(0x30, 0x3b, 31, 0x26, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003a76(0x30, 0x3b, 31, 0x27, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x28, v, 5);
            break;
        case 0x5f: {
            s32 a = 1;
            s32 b = 10;
            Func_02003a9e(0x30, 0x31, 31, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade0;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003a9a() * 40) >> 16) + 105) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Func_02003aca() & 3) != 0) {
        u16 v = Data_0200adec;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003c6e(0xbb);
            a = 1;
            b = 5;
            Func_02003b22(0x2e, 0x3b, 41, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003b34(0x2e, 0x3b, 41, 0x24, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003b54(0x2e, 0x3b, 41, 0x25, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003b74(0x2e, 0x3b, 41, 0x26, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003b9a(0x2e, 0x3b, 41, 0x27, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x28, v, 5);
            break;
        case 0x55: {
            s32 a = 1;
            s32 b = 10;
            Func_02003bc2(0x2e, 0x31, 41, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200adec;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003bbe() * 40) >> 16) + 95) {
                *c = 0;
            }
        }
    }
}
