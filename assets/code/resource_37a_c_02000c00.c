typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

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

extern EntA *Func_020035f2(s32);
extern s32 Func_020035c2(s32);
extern s32 Func_020035ce(s32);
extern void Func_02000d2a(void);
extern s32 Func_020035de(s32);
extern s32 Func_02003608();
extern void Func_02003660(s32, s32);
extern void Func_02003696(s32);
extern u8 Value_00001000;
extern s32 Func_020035fe(s32);
extern EntA *Func_0200364a(s32);
extern void Func_02003688(s32, s32, s32);
extern s32 Func_0200363e();
extern void Func_02003674(s32, s32, s32);
extern void Func_020036c2(s32, s32, s32);
extern void Func_02003678(s32);
extern void Func_020036ae(s32, s32, s32);
extern s32 Func_02003676(s32);
extern void Func_020036e8(s32, s32, s32);
extern void Func_02003698(s32);
extern void Func_020036fc(s32, s32, s32);
extern void Func_02003770(s32, s32, s32);
extern void Func_0200377c();
extern void Func_0200377e(s32, s32, s32);
extern void Func_02003746(s32, s32);
extern s32 Func_020036d6(s32);
extern s32 Func_020036e0(s32);
extern void Func_0200375c(s32, s32);
extern EntB *Func_0200372a(s32);
extern void Func_0200374c(s32, s32, s32);
extern void Func_0200376a(s32);
extern void Func_02003778(s32, s32, s32);
extern void Func_02003786(s32, s32, s32);
extern void Func_0200375a(void);

void Func_02000c00(void)
{
    EntA *obj;
    EntA *a;
    EntB *b;
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

    obj = Func_020035f2(16);
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
    Func_02003608();
    Func_02003660(0, 0);
    Func_02003696((s32)&Value_00001000);
    if (Func_020035fe(g1) != 0 || Func_02003608(0x80a) == 0) {
        a = Func_0200364a(0);
        if (a != 0) {
            Func_02003688(16, a->unk8, a->unk10);
        }
        Func_0200363e(4);
        Func_02003674(16, s1, s2);
    } else {
        if (Func_0200363e(g2) != 0) goto do1;
        h1 = 0x1540000;
        if (obj->unk8 > h1) {
do1:
            Func_020036c2(16, 0x1880000, 0xa80000);
            Func_02003678(4);
            Func_020036ae(16, s3, s4);
        }
    }
    if (Func_02003676(g3) != 0) goto do2;
    h2 = 0x1540000;
    if (obj->unk8 > h2) {
do2:
        Func_020036e8(16, d1, 0xe8);
    } else {
        Func_02003698(0x80a);
    }
    Func_020036fc(16, d2, 0xe8);
    Func_02003770(0, d5, 0);
    Func_0200377c(16, s5, 10);
    Func_0200377e(16, 0, 10);
    Func_02003746(0, 3);
    if (Func_020036d6(g4) != 0 || Func_020036e0(0x80a) == 0) {
        Func_0200375c(16, 2);
        b = Func_0200372a(0);
        if (b != 0) {
            Func_0200374c(16, b->unkA, b->unk12);
        }
        Func_0200376a(16);
        Func_0200377c(16, 0, 0);
        Func_02003778(0, d3, 0xe8);
    } else {
        Func_02003786(0, d4, 0xf8);
    }
    Func_0200375a();
}
