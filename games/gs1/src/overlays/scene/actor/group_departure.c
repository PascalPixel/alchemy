#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/group_departure.h"

#define ACTOR_ID 22

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[92];
    s16 counter;
};

typedef struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Obj *f68;
} Obj;

typedef struct Ent {
    u8 pad00[9];
    u8 b01:2;
    u8 f:2;
    u8 b45:4;
} Ent;

typedef struct Rec {
    u8 pad00[0x50];
    Ent *f50;
} Rec;

struct FixedPointCountdown {
    u8 pad_00[0x18];
    s32 fixed_point_18;
    s32 fixed_point_1c;
    u8 pad_20[0x44];
    s16 countdown;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 Value_00000101;
extern u8 gOv7[];
extern u8 Value_000002d7;
extern u8 Value_00000205;
extern u8 Value_00000109;
extern u8 Value_00000206;
extern u8 Value_00000831;
extern u8 Value_00000832;
extern u8 Value_00000833;
extern u8 Value_00000837;
extern u8 Value_00000838;
extern u8 Value_00000841;
extern u8 Value_00000842;
extern u8 Value_0000083a;
extern u8 Value_0000087b;
extern u8 Value_00000834;
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gCell[];
extern u8 gWork[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u32 gIw;
extern u8 gVal4[];
extern u8 gOv11[];
extern u8 Value_00000e8c;
extern u8 Value_0000201a;
extern u8 Value_00001120;
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 Value_0000030d;

Rec *Actor_Run22(s32);
Rec *Actor_Run23(s32);
Rec *Actor_Run24(s32);

u8 *Actor_Run25(s32);

u8 *Actor_Run26(s32);
u8 *Actor_Run27(s32);
u8 *Actor_Run28(s32);
u8 *Actor_Run29(s32);
u8 *Actor_Run30(s32);
u8 *Actor_Run31(s32);
u8 *Actor_Run32(s32);
u8 *Actor_Run33(s32);

u8 *Actor_Run34(s32);

u8 *Actor_Run35(s32);

u8 *Actor_Run36(s32);

u8 *Actor_Run37(s32);

u8 *Actor_Run38(s32);

u8 *Actor_Run39(s32);
u8 *Actor_Run40(s32);
u8 *Actor_Run41(s32);

u8 *Actor_Run42(s32);

u8 *Actor_Run43(s32);

u8 *Actor_Run44(s32);

u8 *Actor_Run45(s32);

u8 *Actor_Run46();

u8 *Actor_Run47();

u8 *Actor_Run48();

u8 *Actor_Run49();

Thing1 *Actor_Run50(s32);

Thing2 *Actor_Run51(s32);

struct Actor *Actor_Run52(s32);

u8 *Actor_Run53(s32);

u8 *Actor_Run54(s32);

u8 *Actor_Run55(s32);

u8 *Actor_Run56(s32);

u8 *Actor_Run57(s32);

u8 *Actor_Run58(s32);

u8 *Actor_Run59(s32);

u8 *Actor_Run60(s32);

u8 *Actor_Run61(s32);
u8 *Actor_Run62(s32);

u8 *Actor_Run63(s32);
u8 *Actor_Run64(s32);

u8 *Scene_GetRecord_1();

u8 *Actor_Run65(s32);

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

s32 OvObj_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Actor_Check35();
        counter = Actor_Apply(Actor_Check36(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state)
{
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += -0x1000;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16)(UnsignedRemainder(NextRandomValue(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}

u8 *SceneData_GetTableD0E4(void)
{
    return gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return gOv3;
}

u8 *SceneData_GetTableD2B8(void)
{
    return gOv4;
}

u8 *SceneData_GetTableD558(void)
{
    return gOv5;
}

void State_SetFlag210AndConfigureRegion40_84(void)
{
    s32 a;
    s32 b;

    Actor_Do27(0x210);
    a = 10;
    b = 84;
    Actor_SetRect21(40, 84, 7, 4, a, b);
}

void State_SetFlag210AndConfigureRegion40_89(void)
{
    void Actor_Run66(s32);

    s32 a;
    s32 b;

    Actor_Do28(0x210);
    a = 10;
    b = 84;
    Actor_SetRect22(40, 89, 7, 4, a, b);
}

void State_SetWork1c0AndRunObject(u8 *o)
{
    void Actor_Run66(s32);

    u8 *state;

    if (Actor_Check37((s32)&Value_00000834) != 0) {
        Actor_Run67();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Actor_Do29(o);
}

void Scene_SetupDescriptorD774(void)
{
    Actor_Do30(0x9E);
    Actor_Place97(gOv6, 45, 11);
    Actor_Place98(0, (s32)&Value_00000101, 0x1A4);
    Actor_Do31(3);
    Actor_Do32(11);
}

void State_SetValue123Mode1(void)
{
    void Actor_Run66(s32);

    Actor_Do33(0x7B);
    Actor_Do34(1);
}

void State_ApplyValues123And3(void)
{
    void Actor_Run66(s32);

    Actor_Do35(0x7B);
    Actor_Do36(3);
}

void State_SetValue123Mode4(void)
{
    void Actor_Run66(s32);

    Actor_Do37(0x7B);
    Actor_Run66(4);
}

void Scene_RunStep7BAndCheckFlags841And842(void)
{
    Actor_Do38(0x7B);
    if (Actor_Check38((s32)&Value_00000841) != 0
        && Actor_Check39((s32)&Value_00000842) == 0) {
        Actor_Run68();
    }
    Actor_Do39(2);
}

void Scene_SetupDescriptorD78a(void)
{
    Actor_Do40(0x9E);
    Actor_Place99(gOv7, 54, 32);
    Actor_Place100(0, 0x196, (s32)&Value_000002d7);
    Actor_Do41(3);
    Actor_Do42(5);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    if (Actor_Check(0x206) == 0) {
        Actor_Run69(158);
        Actor_Place(0x200d7a0, 45, 39);
    }
    if (Actor_Check2(0x835) == 0) {
        record = Actor_Check3(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        Actor_Run70();
        Actor_Do(0x206);
    } else {
        L_020002b4:;
        Actor_Place2(0, 0x106, 0x325);
        Actor_Run71(3);
        Actor_Run72(6);
    }
}

void Scene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (Actor_Check40((s32)&Value_00000205) == 0) {
        Actor_Do43(0x9E);
        Actor_Place101(gOv7, 50, 44);
    }
    Actor_Place102(0, 0x154, 0x378);
    Actor_Do44(3);
    Actor_Do45(7);
}

void Scene_SetupWithDescriptorD7A0(void)
{
    Actor_Do46(158);
    Actor_Place103((void *)0x0200d7a0, 49, 69);
    Actor_Place104(0, 0x146, 0x466);
    Actor_Do47(3);
    Actor_Do48(8);
}

void Scene_SetupDescriptorD7b6(void)
{
    Actor_Do49(158);
    Actor_Place105((void *)0x0200d7b6, 52, 76);
    Actor_Place106(0, 0x176, 0x4d6);
    Actor_Do50(3);
    Actor_Do51(9);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_Run73(158);
    Actor_Place3(0x200d78a, 35, 74);
    Actor_Place4(0, 102, 0x4b6);
    Actor_Run74(3);
    Actor_Run75(10);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_Run76(158);
    Actor_Place5(0x200d78a, 35, 73);
    Actor_Place6(0, 102, 0x4b6);
    Actor_Run77(3);
    Actor_Run78(12);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_Run79(158);
    Actor_Place7(0x200d7a0, 38, 72);
    Actor_Place8(0, 146, 0x49e);
    Actor_Run80(3);
    Actor_Run81(13);
}

s32 Scene_RunFlagGatedActorSetup(void)
{
    s32 m;
    s32 t;
    s32 m2;
    s32 h;
    s32 k;
    s32 w1 = 0x14E0000;
    s32 w2 = 0x3A40000;
    s32 w3 = 0xE00000;
    s32 w4 = 0x3680000;
    s32 w5 = 0x400000;
    s32 w6 = 0x1B00000;
    s32 w7 = 0x720000;
    s32 w8 = 0xC00000;
    s32 w9 = 0x2000;
    s32 w10 = 0xE30000;
    s32 w11 = 0x4000;
    s32 w12 = 0xF70000;
    s32 w13 = 0x4000;
    s32 w14 = 0xF30000;
    s32 w15 = 0x1900000;
    s32 w16 = 0x1A80000;
    s32 w17 = 0x190;
    s32 w18 = 0x1A8;
    s32 w19 = 0x1A80000;
    s32 w20 = 0x1A8;
    s32 b1 = 0x4BE0000;
    s32 b2 = 0x4BE0000;
    s32 b3 = 0x4BE0000;
    s32 w21 = 0xA50000;
    s32 w22 = 0xA50000;
    s32 p1 = 0x2BF0000;
    s32 p2 = 0x47B0000;
    s32 p3 = 0x14D0000;
    s32 p4 = 0x4FD0000;
    s32 p5 = 0x2630000;
    s32 p6 = 0x2730000;
    s32 p7 = 0x2730000;
    s32 c1 = 0x26B;
    s32 c2 = 0x101;
    s32 c3 = 0x26B;

    Actor_Do52(0xAA);
    Actor_Place107(23, 0, 0);
    if (Actor_Check41((s32)&Value_00000109) != 0) {
        Actor_Do53((s32)&Value_00000205);
        Actor_Do54((s32)&Value_00000206);
    }
    if (Actor_Check42(0x830) != 0) {
        Actor_Place108(11, w1, w2);
        Actor_Run82();
    }
    if (Actor_Check43((s32)&Value_00000831) != 0) {
        Actor_Place109(12, w3, w4);
        Actor_Run83();
    }
    if (Actor_Check44((s32)&Value_00000832) != 0) {
        Actor_Place110(13, w5, p1);
        Actor_Run84();
    }
    if (Actor_Check45((s32)&Value_00000833) != 0) {
        Actor_Place111(14, w6, p2);
        Actor_Run85();
    }
    {
        u8 *q;
        q = Actor_Run26(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Actor_Run27(12);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run28(13);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run29(14);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run30(15);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run31(16);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run25(17);
        q += 0x59;
        *q = *q | m;
        q = Actor_Run32(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (Actor_Check46((s32)&Value_00000837) != 0) {
        Actor_Place112(22, 0, 0);
    }
    {
        u8 *q;
        q = Actor_Run34(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (Actor_Check47((s32)&Value_00000838) != 0) {
        Actor_Place113(19, w7, p3);
    } else {
        Actor_Apply2(19, gOv8);
    }
    if (Actor_Run33((s32)&Value_00000841) != 0) {
        s32 h2;
        u8 *tbl;
        Actor_Run86();
        Actor_Place114(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Actor_Run35(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply3(Actor_Check48(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = gOv9;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Actor_Apply4(9, tbl);
        }
        Actor_Place115(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_Run36(26);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply5(Actor_Check49(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Actor_Apply6(26, tbl);
        }
        Actor_Place116(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_Run37(22);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply7(Actor_Check50(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Actor_Apply8(22, tbl);
        }
        Actor_Place117(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_Run38(8);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply9(Actor_Check51(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Actor_Apply10(8, tbl);
        }
        Actor_Apply11(8, 6);
        {
            u8 *r;
            r = Actor_Run39(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Actor_Run40(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Actor_Apply12(gOv10, 0xC80);
        Actor_Place118(24, 0, 0);
        Actor_Place119(25, 0, 0);
        Actor_Place120(23, 0, 0);
        Actor_Place121(19, 0, 0);
        if (Actor_Check52((s32)&Value_00000842) != 0) {
            Actor_Place122(22, 0, 0);
        }
    } else if (Actor_Run41((s32)&Value_0000083a) != 0) {
        u8 *tbl;
        Actor_Place123(10, w8, b1);
        Actor_Place124(10, w9, 0);
        Actor_Apply13(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_Run42(10);
            v = Actor_Apply14(Actor_Check53(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = gOv9;
        Actor_Apply15(10, tbl);
        Actor_Place125(24, w10, b2);
        Actor_Place126(24, w11, 0);
        Actor_Apply16(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Run43(24);
            v = Actor_Apply17(Actor_Check54(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_Apply18(24, tbl);
        Actor_Place127(25, w12, b3);
        Actor_Place128(25, w13, 0);
        Actor_Apply19(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_Run44(25);
            v = Actor_Apply20(Actor_Check55(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_Apply21(25, tbl);
        Actor_Place129(23, w14, p4);
        Actor_Place130(23, 0xC000, 0);
        Actor_Apply22(Actor_Check56(23), 0);
        Actor_Place131(17, 0, 0);
        Actor_Place122(18, 0, 0);
    } else {
        Actor_Place132(17, 0, 0);
        Actor_Place133(18, 0, 0);
    }
    {
        s16 *table = (s16 *)gCell;
        if (table[225] != 15 || Actor_Check57((s32)&Value_0000087b) != 0) {
            Actor_Run87();
            Actor_Run88();
        }
    }
    if (Actor_Check58(0x210) != 0) {
        Actor_Run89();
    }
    Actor_Check59((s32)&Value_00000834);
    k = 46;
    Actor_SetRect23(29, 24, 1, 2, 26, k);
    Actor_SetRect24(29, 25, 1, 1, 27, k);
    Actor_SetRect25(29, 25, 1, 1, 28, k);
    k = 20;
    Actor_SetRect26(19, 0x5A, 1, 1, k, 0x58);
    Actor_SetRect27(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Actor_Run45(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Actor_Apply23(o, 0);
    }
    Actor_Check60(1);
    if (Actor_Check61((s32)&Value_0000087b) == 0) {
        s16 *table = (s16 *)gCell;
        if (table[225] == 15) {
            Actor_Run90();
            return 0;
        }
    }
    Actor_Apply24(23, 7);
    if (Actor_Check62((s32)&Value_00000837) == 0) {
        Actor_Run91();
        Actor_Apply25(22, c2);
        Actor_Place134(22, w15, p5);
        Actor_Place135(21, w16, p6);
        Actor_Place136(22, w17, c1);
        Actor_Place137(21, w18, 0x26B);
        Actor_Apply26(21, 2);
        Actor_Apply27(22, 5);
        Actor_Run92();
    } else {
        Actor_Run93();
        Actor_Place138(21, w19, p7);
        Actor_Place139(21, w20, c3);
        Actor_Apply28(21, 3);
        Actor_Run94();
    }
    {
        u8 *state = *(u8 **)0x03001EBC;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Actor_Run95();
    Actor_Run96();
    Actor_Run97();
    return 0;
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_e5c;

    Actor_Run98();
    Actor_Run99();
    Actor_Run100();
    Actor_Run101();
    Actor_Run102(60);
    Actor_Run(0x4000, 0x800);
    Actor_Run2(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_Place9(10, 0x1260000, 0x3640000);
    Actor_Run103(0, 0, 0);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    Actor_Run104();
    Actor_Run105();
    Actor_Run106();
    Actor_Run107(158);
    Actor_Place10(0x200d78a, 50, 44);
    Actor_Run3(22, 0x101);
    Actor_Place11(9, 0xcccc, 0x6666);
    Actor_Place12(0, 0xcccc, 0x6666);
    Actor_Place13(10, 0xcccc, 0x6666);
    Actor_Place14(9, 0x1560000, 0x37a0000);
    Actor_Place15(9, 0x156, 0x389);
    Actor_Run108();
    Actor_Place16(9, 0x128, 0x389);
    Actor_Place17(0, 0x1560000, 0x37a0000);
    Actor_Place18(0, 0x156, 0x37a);
    Actor_Place19(0, 0x156, 0x389);
    Actor_Place20(0, 0x13e, 0x389);
    Actor_Run109(9, 1);
    Actor_Run110(9, 1);
    Actor_Place21(9, 0xc000, 60);
    base5_e5c = (s32)gVal;
    Actor_Run111(base5_e5c);
    Actor_Run112(9, 0);
    Actor_Place22(10, 0x126, 0x346);
    Actor_Run113(40);
    Actor_Run114(10, 4);
    Actor_Run115(10, 0);
    Actor_Run116(0, 9, 0);
    Actor_Run117(40);
    Actor_Place23(10, 0x4000, 0);
    Actor_Run118(10, 0, 20);
    Actor_Place24(9, 0x101, 20);
    Actor_Place25(9, 0xc000, 10);
    Actor_Run119(9, 0, 10);
    Actor_Run120(10, 4);
    Actor_Run121(10, 0);
    Actor_Run4(9, 0x102);
    Actor_Run122(30);
    Actor_Run123(9, 0, 50);
    Actor_Place26(9, 0xc000, 10);
    Actor_Place27(9, 0x18000, 0xc000);
    Actor_Place28(9, 0x121, 0x373);
    Actor_Place29(9, 0xe000, 0);
    Actor_Run124(9, 0);
    Actor_Run125(10, 2);
    Actor_Run126(10, 0);
    Actor_Run127(9, 4);
    Actor_Run128(9, 0, 10);
    Actor_Place30(9, 0x2000, 10);
    Actor_Run129((base5_e5c + 8));
    Actor_Check4(9, 0);
    Actor_Place31(0, 0x12e, 0x389);
    Actor_Place32(0, 0xc000, 0);
    while (Actor_Check5(0, 0) == 1) {
        Actor_Run130(9, 1);
        Actor_Do2(0xe65);
        Actor_Run131(9, 0);
    }
    Actor_Run132(9, 3);
    Actor_Do3(0xe66);
    Actor_Run133(9, 0, 10);
    Actor_Run134(0, 3);
    Actor_Place33(10, 0x18000, 0xc000);
    Actor_Place34(10, 0x129, 0x2ee);
    Actor_Run135(10);
    Actor_Place35(9, 0x129, 0x2ee);
    Actor_Run136(9, 0, 0);
    Actor_Run137(10, 0, 0);
    Actor_Run138(10, 1);
    Actor_Run139(21, 2);
    Actor_Run140(22, 5);
    Actor_Do4(0x12f);
    Actor_Do5(0x87b);
    Actor_Do6(0x205);
    Actor_Run141();
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_SetRect(29, 64, 1, 1, 21, 57);
    Actor_SetRect2(29, 64, 1, 1, 21, 58);
    Actor_SetRect3(29, 64, 1, 1, 22, 58);
    Actor_SetRect4(29, 64, 1, 1, 20, 58);
    Actor_SetRect5(28, 20, 1, 1, 20, 57);
}

void ActorDraw_SetEightSceneCells(void)
{
    extern s32 gOv12;

    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Actor_SetRect28(29, 23, 1, 1, a, d);
    b = 14;
    Actor_SetRect29(29, 23, 1, 1, b, d);
    c = 13;
    Actor_SetRect30(29, 23, 1, 1, c, d);
    Actor_SetRect31(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Actor_SetRect32(25, 21, 1, 1, c, e);
    Actor_SetRect33(25, 21, 1, 1, a, e);
    Actor_SetRect34(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Actor_SetRect35(13, 0x37, 1, 1, a, f);
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 rec8;
    u8 *record;
    s32 r8;
    s32 v5;
    u8 *p5;
    u8 *later;

    if (Actor_Check6(0x312) != 0) {
        v5 = r8;
    } else {
        Actor_Run142();
        v5 = r8;
        if (Actor_Check7(0x832) == 0) {
            rec8 = Actor_Check8(13);
            record = Actor_Run46(0);
            p5 = (u8 *)(s32)*(volatile u8 *)(*(volatile s32 *)((s32)record + 80) + 9);
            record += 35;
            p10 = *record;
            Actor_Place36(0x40000, 0x40000, 0x10000);
            Actor_Run143(141);
            Actor_Run144(40);
            Actor_Run145(145);
            Actor_Run146(0, 3);
            {
                u8 *record = Actor_Run47(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
            Actor_Place37(13, 0, 0x2bf0000);
            *(volatile s32 *)(rec8 + 48) = 0x18000;
            *(volatile s32 *)(rec8 + 52) = 0x18000;
            {
                s32 shifted = *(volatile s32 *)(rec8 + 12) + 0x500000;

                *(volatile s32 *)(rec8 + 12) = shifted;
                *(volatile s32 *)(rec8 + 60) = shifted;
            }
            *(volatile s32 *)(rec8 + 68) = 0x8000;
            Actor_Place38(13, 64, 0x2bf);
            Actor_Run147(40);
            Actor_Do7(0x121);
            Actor_Place39(-1, -1, 0xe666);
            Actor_Run148();
            Actor_Run149();
            Actor_Do8(0x832);
            Actor_Run150(0, ((u32)(s32)((s32)p5 << 28) >> 30));
            {
                u8 *record = Actor_Run48(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 1);
            }
            *record = p10;
        }
        Actor_Run151();
        Actor_Do9(0x312);
        if (Actor_Check9(0x837) != 0) {
            if (Actor_Check10(0x841) == 0) {
                if (Actor_Check11(0x30c) == 0) {
                    later = Actor_Run49(0);
                    if (*(volatile s32 *)((s32)later + 16) <= 0x2b4ffff) {
                        Actor_Check12(62, 0x29d);
                        Actor_Place40(0, 27, 0x273);
                    } else {
                        Actor_Check13(75, 0x2cb);
                        Actor_Place41(0, 67, 0x2f5);
                    }
                    Actor_Do10(0x30c);
                }
            }
        }
        Actor_Run152();
    }
    p10b = v5;
}

void State_ApplyFourRects(void)
{
    void Actor_Run66(s32);

    s32 a = 0x2a;
    s32 b;

    Actor_SetRect36(29, 22, 1, 1, 3, a);
    b = 2;
    Actor_SetRect37(29, 21, 1, 1, b, a);
    Actor_SetRect38(29, 21, 1, 1, 4, a);
    Actor_SetRect39(23, 20, 3, 1, b, 0x2b);
}

void Scene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Actor_SetRect40(29, 20, 1, 1, a, h);
    b = 0x46;
    Actor_SetRect41(29, 20, 1, 1, a, b);
    a2 = 27;
    Actor_SetRect42(29, 20, 1, 1, a2, b);
    Actor_SetRect43(28, 21, 1, 1, 28, h);
    Actor_SetRect44(28, 22, 1, 1, a2, 0x48);
}

void Scene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Actor_Run153();
    Actor_Place42(0, 0x106, 0x32a);
    Actor_Place43(20, 0x1060000, 0x3250000);
    Actor_Place44(20, 0x106, 0x339);
    Actor_Place45(0, 0x20000, 0x10000);
    Actor_Run154(0, 2, 0);
    Actor_Place46(0, 0x11a, 0x357);
    Actor_Run155(20, 1);
    Actor_Run156(0, 4, 0);
    Actor_Run157(0, 20, 0);
    Actor_Run158();
    Actor_Run159(30);
    Actor_Run160(0, 2);
    Actor_Place47(20, 0x100, 20);
    base5_e67 = (s32)gVal2;
    Actor_Run161(base5_e67);
    Actor_Run162(20, 0);
    Actor_Run163(20);
    Actor_Check14(20, 0);
    Actor_Run164(20, 2);
    Actor_Run165((base5_e67 + 4));
    Actor_Run166(20, 0, 20);
    Actor_Run5(20, 0x200c8c0);
    Actor_Do11(0x835);
    Actor_Run167();
}

void Scene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (Actor_Check15(0x836) == 0) {
        if (Actor_Check16(0x837) == 0) {
            Actor_Run168();
            Actor_Do12(0xe6c);
            Actor_Run169(22, 0, 20);
            Actor_Place48(0, 0x101, 40);
            Actor_Place49(0, 0x17e, 0x26b);
            Actor_Run170(0, 22, 0);
            Actor_Run171(0, 2);
            Actor_Run172(30);
            Actor_Run173(22, 0);
            Actor_Do13(0x836);
            Actor_Run174();
        }
    }
}

void Scene_RunActor22SceneWhenFlag836Only(void)
{
    if (Actor_Check63(0x837) == 0 && Actor_Check64(0x836) != 0) {
        Actor_Run175();
        Actor_Apply29(22, 2);
        Actor_Do55(20);
        Actor_Do56(0xe71);
        Actor_Run176();
        Actor_Run177();
    }
}

void Scene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (Actor_Check17(0x841) != 0) {
        Actor_Run178();
        Actor_Run179(22, 0, 0);
        Actor_Run180(20);
        Actor_Run181((s32)gVal3);
        Actor_Run182(22, 0);
        Actor_Place50(22, 0xe000, 10);
        Actor_Run183();
    } else {
        if (Actor_Check18(0x837) == 0) {
            Actor_Run184();
            Actor_Do14(0xe6e);
            Actor_Run185();
            Actor_Run186();
        }
    }
}

void Scene_RunScene372SequenceE(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (Actor_Check19(0x837) != 0) {
    } else {
        Actor_Run187();
        Actor_Run6(22, 0x100);
        base5_e74 = (s32)gVal4;
        Actor_Run188(base5_e74);
        Actor_Run189(22, 0);
        Actor_Place51(0, 0x100, 20);
        Actor_Place52(0, 0x4000, 0);
        Actor_Run7(0x6666, 0xccc);
        Actor_Run8(0x1000000, -1, 0x24c0000, 1);
        Actor_Place53(22, 0x20000, 0x10000);
        Actor_Check20(22, 0x200c934);
        Actor_Run190(0, 22, 0);
        Actor_Run191(30);
        Actor_Check21(22, 0x200c984);
        Actor_Run192(22, 0);
        v6 = 128;
        record = Actor_Check65(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Actor_Run193(22, 1);
        Actor_Run194(20);
        Actor_Run195(22, 0);
        Actor_Run196(40);
        Actor_Run197(22, 1);
        Actor_Run198((base5_e74 + 5));
        Actor_Run199(22, 0, 20);
        Actor_Run200(0, 3);
        Actor_Run201(22, 3);
        Actor_Run202(22, 0);
        Actor_Place54(22, (v6 << 9), 0x8000);
        Actor_Run203(22, 2);
        record = Actor_Check22(0);
        if (record != 0) {
            Actor_Run204(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_Run205(22);
        Actor_Run206(22, 0, 0);
        Actor_Run207(1, 1);
        Actor_Run208(21, 3);
        Actor_Do15(0x837);
        Actor_Run209();
    }
}

void Scene_RunScene372SequenceD(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 record;

    Actor_Run210();
    record = Actor_Check23(0);
    if (record != 0) {
        Actor_Run211(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Actor_Place55(22, 0x10000, 0x8000);
    Actor_Place56(22, 0x119, 0x1fb);
    Actor_Run212(22, 0, 0);
    Actor_Run213(30);
    Actor_Do16(0xe7b);
    Actor_Run214(22, 0);
    Actor_Run215(0, 22, 0);
    Actor_Run216(10);
    Actor_Run217(0, 1);
    Actor_Run218(20);
    Actor_Place57(22, 0x4000, 0);
    Actor_Run219(22, 0);
    Actor_Run220(22, 2);
    record = Actor_Check24(0);
    if (record != 0) {
        Actor_Run221(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_Run222(22);
    Actor_Run223(22, 0, 0);
    Actor_Place58(0, 0x100, 0x205);
    Actor_Run224();
}

void Actor_RunActor22PlacementSequence(s32 x, s32 y)
{
    extern s32 gOv12;

    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Actor_Run50(0);
    if (a != 0) {
        Actor_Place140(22, a->unk8, a->unk10);
    }
    Actor_Place141(22, w, h);
    Actor_Place142(22, x, y);
    Actor_Place143(0, 22, 0);
    Actor_Do57(20);
    Actor_Apply30(0, 0x102);
    Actor_Do58(40);
    Actor_Do59(0xe7d);
    Actor_Apply31(22, 0);
    Actor_Apply32(22, 2);
    Actor_Apply33(22, 0);
    Actor_Apply34(0, 3);
    Actor_Apply35(22, 2);
    b = Actor_Run51(0);
    if (b != 0) {
        Actor_Place144(22, b->unkA, b->unk12);
    }
    Actor_Do60(22);
    Actor_Place145(22, 0, 0);
}

void Scene_RunReturnScript(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 p10;
    s32 p11;
    s32 p9;
    s32 record;
    s32 r9;
    s32 r10;
    s32 r8;
    s32 base5_200d7fc;
    s32 base7_200c56d;
    s32 base_200c5a9;
    s32 shifted;
    s32 v5;
    s32 v6;
    s32 v7;

    if (Actor_Check25(0x838) != 0) {
        v5 = r9;
        v6 = r10;
        v7 = r8;
    } else {
        Actor_Run225();
        Actor_Do17(0x200d4b0);
        Actor_Run226();
        Actor_Run227(1);
        Actor_Run228(141);
        Actor_Place59(0x20000, 0x20000, 0x10000);
        Actor_Run229(30);
        Actor_Place60(0x30000, 0x30000, 0x10000);
        Actor_Run230(145);
        Actor_Run231(30);
        record = Actor_Check26(0);
        if (record != 0) {
            Actor_Run232(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Actor_Place61(0, 0x20000, 0x10000);
        Actor_Place62(22, 0x20000, 0x10000);
        Actor_Check27(0, 0x200ca00);
        Actor_Run9(22, 0x200ca3c);
        Actor_Run233(0);
        Actor_Place63(0, 0x100, 0);
        Actor_Place64(22, 0x100, 30);
        Actor_Place65(0x40000, 0x40000, 0x10000);
        Actor_Run234(145);
        Actor_Run235(40);
        Actor_Place66(0x50000, 0x50000, 0x10000);
        Actor_Run236(145);
        Actor_Run237(20);
        Actor_Run10(0, 0x102);
        Actor_Run11(22, 0x102);
        Actor_Run238(40);
        Actor_Run239(32, 5);
        Actor_Run240(33, 5);
        Actor_Run241(30, 8);
        Actor_Run242(29, 8);
        record = Actor_Check66(30);
        *(volatile s32 *)(record + 24) = -0x10000;
        Actor_Run243(32, 2);
        Actor_Run244(33, 2);
        Actor_Run245(30, 3);
        Actor_Run246(29, 3);
        Actor_Do18(0xe7f);
        Actor_Run247(28, 0, 20);
        Actor_Place67(0, 0xc000, 0);
        Actor_Place68(22, 0xc000, 20);
        Actor_Run12(0x40000, 0x8000);
        Actor_Run13(0x700000, -1, 0x14b0000, 1);
        Actor_Run248();
        for (i = 0; i < 40; i++) {
            Actor_Run249(Actor_Check67(32));
            Actor_Run250(Actor_Check68(33));
            Actor_Run251(Actor_Check69(30));
            Actor_Run252(Actor_Check70(29));
            Actor_Run253(1);
        }
        base5_200d7fc = (s32)gOv12;
        base7_200c56d = (s32)Actor_Run254;
        v5 = base5_200d7fc;
        v6 = 0;
        v7 = base7_200c56d;
        *(volatile s32 *)gOv11 = 0;
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_Check28(base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Actor_Run14(base_200c5a9, 0xc80);
        Actor_Run255(40);
        *(volatile s32 *)base5_200d7fc = 1;
        Actor_Run256(30);
        Actor_Place69(19, 0x720000, 0x1220000);
        record = Actor_Check71(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Actor_Place70(19, 0xcccc, 0x6666);
        Actor_Run257(145);
        Actor_Place71(19, 114, 0x14d);
        Actor_Run258(19, 2);
        Actor_Place72(0x20000, 0x20000, 0x10000);
        Actor_Run259(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_Place73(19, 0x6666, 0x3333);
        Actor_Place74(19, 114, 0x12c);
        Actor_Run260(19, 2);
        Actor_Place75(0x50000, 0x50000, 0x10000);
        Actor_Run261(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Actor_Place76(19, 0xcccc, 0x6666);
        Actor_Place77(19, 114, 0x14d);
        Actor_Run262(19, 2);
        Actor_Place78(0x20000, 0x20000, 0x10000);
        Actor_Run263(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_Place79(19, 0x6666, 0x3333);
        Actor_Place80(19, 114, 0x12c);
        Actor_Run264(19, 2);
        Actor_Place81(0x40000, 0x40000, 0x10000);
        Actor_Run265(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Actor_Place82(19, 0xcccc, 0x6666);
        Actor_Place83(19, 114, 0x14d);
        Actor_Run266(19, 2);
        Actor_Place84(0x20000, 0x20000, 0x10000);
        Actor_Run267(145);
        *(volatile s32 *)base5_200d7fc = 1;
        Actor_Run268(20);
        Actor_Run15(32, 0x102);
        Actor_Run269(32, 2);
        Actor_Run270(31, 0);
        Actor_Place85(33, 0x100, 0);
        Actor_Run271(33, 2);
        Actor_Run272(28, 0, 40);
        Actor_Run16(30, 0x102);
        Actor_Run273(30, 2);
        Actor_Run274(30, 0);
        *(volatile s32 *)gOv11 = 1;
        Actor_Run275(29, 1);
        Actor_Run276(1);
        Actor_Run277(29, 0);
        Actor_Place86(29, 0x105, 20);
        Actor_Place87(29, 0x8000, 40);
        Actor_Run278(29, 0, 20);
        Actor_Place88(29, 0x8000, 20);
        Actor_Place89(29, 0x4000, 40);
        Actor_Place90(29, 0x100, 0);
        Actor_Run279(29, 2);
        Actor_Run280(29, 4, 40);
        Actor_Run281(29, 9);
        Actor_Run282(10);
        Actor_Run283(29, 0, 20);
        Actor_Do19(0x121);
        Actor_Place91(-1, -1, 0xe666);
        Actor_Run17(0x60000, 0xc000);
        Actor_Run18(0x540000, -1, 0x2340000, 1);
        Actor_Run284();
        Actor_Run285();
        Actor_Run286(22, 0, 0);
        Actor_Run287(20);
        Actor_Run19(22, 0x102);
        Actor_Run288(30);
        Actor_Run289(base7_200c56d);
        Actor_Run290(base_200c5a9);
        Actor_Run291(22, 0);
        Actor_Run292(0, 22, 0);
        Actor_Run293(20);
        Actor_Run294();
        Actor_Run295(0, 3);
        Actor_Run296(22, 3);
        Actor_Run297(20);
        Actor_Run298(22, 2);
        record = Actor_Check29(0);
        if (record != 0) {
            Actor_Run299(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_Run300(22);
        Actor_Run301(22, 0, 0);
        Actor_Run302(31);
        Actor_Run303(28);
        Actor_Run304(30);
        Actor_Run305(29);
        Actor_Run306(32);
        Actor_Run307(33);
        Actor_Do20(0x838);
        Actor_Run308();
    }
    p9 = v5;
    p10 = v6;
    p11 = v7;
}

void OvObj_SetChildByte5AndMark(u8 *o, s32 n)
{
    if ((*(u8 *)(o + 0x54) & 15) == 1) {
        u8 *c = *(u8 **)(o + 0x50);
        s32 idx = n - 1;
        u8 cnt;
        if (n == 0) {
            idx = gOv[(*(u32 *)0x03001E40 >> 1) & (*(u8 *)(o + 0x54) & 15)];
        }
        cnt = *(u8 *)(c + 0x27);
        if (cnt != 0) {
            u8 **p = (u8 **)(c + 0x28);
            s32 k = cnt;
            do {
                u8 *e = *p++;
                if (e != 0 && *(s32 *)(e + 16) != 0) {
                    *(u8 *)(e + 5) = idx;
                }
                k--;
            } while (k != 0);
        }
        *(u8 *)(c + 0x25) = 1;
    }
}

void ActorDraw_SetFourActorsModeByBit(void)
{
    extern s32 gOv12;

    if (((*(u32 *)0x03001E40 >> gOv12) & 3) != 0) {
        Actor_Apply36(Actor_Check72(32), 1);
        Actor_Apply37(Actor_Check73(33), 1);
        Actor_Apply38(Actor_Check74(30), 1);
        Actor_Apply39(Actor_Check75(29), 1);
    } else {
        Actor_Apply40(Actor_Check76(32), 8);
        Actor_Apply41(Actor_Check77(33), 8);
        Actor_Apply42(Actor_Check78(30), 8);
        Actor_Apply43(Actor_Check79(29), 8);
    }
}

void Scene_RunFlagGatedActorSequence(void)
{
    extern u8 gOv12[];

    s32 kc0_1 = 0xC00000;
    s32 k4be_1 = 0x4BE0000;
    s32 k2000_1 = 0x2000;
    s32 kc0_2 = 0xC00000;
    s32 k4a5_1 = 0x4A50000;
    s32 k2000_2 = 0x2000;
    s32 ke3_1 = 0xE30000;
    s32 k4be_2 = 0x4BE0000;
    s32 k4000_1 = 0x4000;
    s32 kfa_1 = 0xFA0000;
    s32 k4be_3 = 0x4BE0000;
    s32 k4000_2 = 0x4000;
    s32 ke3_2 = 0xE30000;
    s32 k4a5_2 = 0x4A50000;
    s32 k2000_3 = 0x3000;
    s32 kf3_1 = 0xF30000;
    s32 k4fd_1 = 0x4FD0000;
    s32 kc000_1 = 0xC000;
    s32 k100_1 = 0x100;
    s32 k446_1 = 0x446;
    s32 k446_2 = 0x446;
    s32 k4000_3 = 0x4000;
    s32 k4000_4 = 0x4000;
    s32 k40000_1 = 0x40000;
    s32 k8000_1 = 0x8000;
    s32 kd8_1 = 0xD80000;
    s32 ng1 = -1;
    s32 k4d0_1 = 0x4D00000;
    s32 k3000_1 = 0x3000;
    s32 k30000_1 = 0x30000;
    s32 k6000_1 = 0x6000;
    s32 ke8_1 = 0xE80000;
    s32 ng2 = -1;
    s32 k4e5_1 = 0x4E50000;
    s32 k9999_1 = 0x9999;
    s32 k1333_1 = 0x1333;
    s32 kd8_2 = 0xD80000;
    s32 ng3 = -1;
    s32 k4d0_2 = 0x4D00000;
    s32 k102_1 = 0x102;
    s32 k102_2 = 0x102;
    s32 k4b5_1 = 0x4B5;
    s32 k4b1_1 = 0x4B1;
    s32 ke8_2 = 0xE80000;
    s32 ng4 = -1;
    s32 k4e5_2 = 0x4E50000;
    s32 kf3_2 = 0xF30000;
    s32 k4fd_2 = 0x4FD0000;
    s32 k20000_1 = 0x20000;
    s32 k19999_1 = 0x19999;
    s32 k3333_1 = 0x3333;
    s32 kd8_3 = 0xD80000;
    s32 ng5 = -1;
    s32 k4d0_3 = 0x4D00000;
    s32 k105_1 = 0x105;
    s32 k800a_1 = 0x800A;
    s32 k9999_2 = 0x9999;
    s32 k4ccc_1 = 0x4CCC;
    s32 k9999_3 = 0x9999;
    s32 k4ccc_2 = 0x4CCC;
    s32 k4ba_1 = 0x4BA;
    s32 k4a5_3 = 0x4A5;
    s32 k6000_2 = 0x6000;
    s32 k8000_2 = 0x8000;
    s32 k8018_1 = 0x8018;
    s32 kc000_2 = 0xC000;
    s32 k800a_2 = 0x800A;
    s32 k105_2 = 0x105;
    s32 k105_3 = 0x105;
    s32 k106_1 = 0x106;
    s32 k8000_3 = 0x8000;
    s32 kc000_3 = 0xC000;
    s32 k4000_5 = 0x4000;
    s32 kc000_4 = 0xC000;
    s32 k9000_1 = 0x9000;
    s32 ka000_1 = 0xA000;
    s32 k8000_4 = 0x8000;
    s32 k800a_3 = 0x800A;
    s32 k105_4 = 0x105;
    s32 k105_5 = 0x105;
    s32 k105_6 = 0x105;
    s32 k105_7 = 0x105;
    s32 k8000_5 = 0x8000;
    s32 k8000_6 = 0x8000;
    s32 k800a_4 = 0x800A;
    s32 kd000_1 = 0xD000;
    s32 k2000_4 = 0x2000;
    s32 ka000_2 = 0xA000;
    s32 k8000_7 = 0x8000;
    s32 v83a_2 = 0x83A;
    u8 *tbl;
    s32 w16;
    s32 m;
    s32 one;

    if (Actor_Check80((s32)&Value_0000083a) != 0) {
        return;
    }
    Actor_Run309();
    Actor_Place146(10, kc0_1, k4be_1);
    Actor_Place147(10, k2000_1, 0);
    Actor_Apply44(10, 5);
    {
        u8 *o;
        s32 v;
        o = Actor_Run53(10);
        v = Actor_Apply45(Actor_Check81(), 0x5A) + 60;
        tbl = gOv9;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply46(10, tbl);
    }
    Actor_Place148(9, kc0_2, k4a5_1);
    Actor_Place149(9, k2000_2, 0);
    Actor_Place150(24, ke3_1, k4be_2);
    Actor_Place151(24, k4000_1, 0);
    Actor_Apply47(24, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Run54(24);
        v = Actor_Apply48(Actor_Check82(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply49(24, tbl);
    }
    Actor_Place152(25, kfa_1, k4be_3);
    Actor_Place153(25, k4000_2, 0);
    Actor_Apply50(25, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Run55(25);
        v = Actor_Apply51(Actor_Check83(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply52(25, tbl);
    }
    Actor_Place154(26, ke3_2, k4a5_2);
    Actor_Place155(26, k2000_3, 0);
    Actor_Place156(23, kf3_1, k4fd_1);
    Actor_Place157(23, kc000_1, 0);
    Actor_Apply53(Actor_Check84(23), 0);
    Actor_Do61(3);
    Actor_Do62((s32)&Value_00000e8c);
    Actor_Apply54((s32)&Value_0000201a, 0);
    Actor_Place158(0, k100_1, 20);
    Actor_Place159(0, 150, k446_1);
    {
        u8 *p;
        p = Actor_Run56(0);
        if (p != 0) {
            Actor_Place160(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Actor_Place161(22, 132, k446_2);
    Actor_Place162(0, 22, 0);
    Actor_Do63(40);
    Actor_Place163(0, k4000_3, 0);
    Actor_Place164(22, k4000_4, 20);
    Actor_Apply55(k40000_1, k8000_1);
    Actor_SetMode(kd8_1, ng1, k4d0_1, 1);
    Actor_Run310();
    Actor_Do64(40);
    Actor_Apply56(10, 2);
    Actor_Place165(10, 0, 10);
    Actor_Apply57(23, 3);
    Actor_Place166(9, 0, 10);
    Actor_Apply58(9, 3);
    Actor_Place167(9, 0, 10);
    Actor_Place168(9, k3000_1, 10);
    Actor_Apply59(k30000_1, k6000_1);
    Actor_SetMode2(ke8_1, ng2, k4e5_1, 1);
    Actor_Run311();
    Actor_Do65(20);
    Actor_Do66(134);
    Actor_Place169(23, 4, 0);
    Actor_Apply60(23, 6);
    Actor_Do67(10);
    Actor_Place170(23, 0, 0);
    Actor_Do68(60);
    Actor_Run312();
    Actor_Apply61(10, 1);
    {
        u8 *o;
        o = Actor_Run57(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_Apply62(24, 1);
    {
        u8 *o;
        o = Actor_Run58(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_Apply63(25, 1);
    {
        u8 *o;
        o = Actor_Run59(25);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_Apply64(10, 2);
    Actor_Apply65(9, 2);
    Actor_Apply66(24, 2);
    Actor_Apply67(25, 2);
    Actor_Apply68(26, 2);
    Actor_Apply69(k9999_1, k1333_1);
    Actor_SetMode3(kd8_2, ng3, k4d0_2, 1);
    Actor_Run313();
    Actor_Apply70(26, k102_1);
    Actor_Apply71(9, k102_2);
    Actor_Do69(60);
    Actor_Apply72(26, 2);
    Actor_Apply73(26, 3);
    Actor_Apply74(26, 0);
    Actor_Place171(25, 2, 0);
    Actor_Place172(25, 234, k4b5_1);
    Actor_Place173(26, 2, 0);
    Actor_Place174(26, 227, k4b1_1);
    Actor_Do70(90);
    Actor_SetMode4(ke8_2, ng4, k4e5_2, 1);
    Actor_Run314();
    Actor_Place175(23, kf3_2, k4fd_2);
    Actor_Do71(1);
    Actor_Do72(106);
    {
        u8 *o;
        o = Actor_Run60(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Actor_Do73(6);
    Actor_Apply75(23, 7);
    Actor_Do74(20);
    Actor_Run315();
    Actor_Do75(20);
    Actor_Apply76(k19999_1, k3333_1);
    Actor_SetMode5(kd8_3, ng5, k4d0_3, 1);
    Actor_Run316();
    Actor_Apply77(24, 2);
    Actor_Do76(20);
    Actor_Place176(24, k105_1, 40);
    Actor_Place177(24, 10, 0);
    Actor_Do77(10);
    Actor_Apply78(10, 2);
    Actor_Apply79(k800a_1, 0);
    {
        u8 *o;
        o = Actor_Run61(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Run62(26);
        o += 0x5A;
        *o = *o & m;
    }
    Actor_Place178(25, k9999_2, k4ccc_1);
    Actor_Place179(26, k9999_3, k4ccc_2);
    Actor_Place180(25, 247, k4ba_1);
    Actor_Place181(26, 227, k4a5_3);
    {
        u8 *o;
        o = Actor_Run63(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Run64(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Actor_Place182(26, k6000_2, 0);
    Actor_Place183(25, k8000_2, 10);
    Actor_Apply80(24, 4);
    Actor_Place184(k8018_1, 0, 10);
    Actor_Place185(10, kc000_2, 20);
    Actor_Place186(10, 0, 10);
    Actor_Apply81(10, 4);
    Actor_Place187(k800a_2, 0, 10);
    Actor_Place188(24, k105_2, 0);
    Actor_Place189(10, k105_3, 60);
    Actor_Place190(9, k106_1, 20);
    Actor_Place191(9, k8000_3, 40);
    Actor_Place192(9, kc000_3, 20);
    Actor_Place193(9, 0, 30);
    Actor_Place194(9, k4000_5, 10);
    Actor_Place195(9, 0, 10);
    Actor_Place196(10, kc000_4, 0);
    Actor_Place197(25, k9000_1, 0);
    Actor_Place198(24, ka000_1, 0);
    Actor_Place199(26, k8000_4, 10);
    Actor_Apply82(10, 1);
    Actor_Place200(k800a_3, 0, 10);
    Actor_Apply83(9, 4);
    Actor_Place201(9, 0, 10);
    Actor_Place202(10, k105_4, 0);
    Actor_Place203(24, k105_5, 0);
    Actor_Place204(25, k105_6, 0);
    Actor_Place205(26, k105_7, 40);
    Actor_Place206(9, 0, 10);
    Actor_Place207(24, 25, 0);
    Actor_Do78(20);
    Actor_Place208(9, 0, 0);
    Actor_Place209(10, 0, 10);
    Actor_Place210(24, k8000_5, 0);
    Actor_Place211(25, k8000_6, 10);
    Actor_Apply84(24, 3);
    Actor_Apply85(25, 3);
    Actor_Place212(10, 9, 0);
    Actor_Do79(20);
    Actor_Apply86(10, 1);
    Actor_Place213(k800a_4, 0, 10);
    Actor_Apply87(9, 3);
    Actor_Place214(24, kd000_1, 10);
    Actor_Apply88(24, 1);
    Actor_Place215(24, 0, 10);
    Actor_Place216(10, 0, 0);
    Actor_Place217(9, 0, 0);
    Actor_Apply89(26, 1);
    Actor_Place218(26, k2000_4, 20);
    Actor_Place219(25, ka000_2, 20);
    Actor_Apply90(25, 3);
    Actor_Place220(25, 0, 10);
    Actor_Apply91(26, 3);
    Actor_Do80(20);
    Actor_Apply92(9, 2);
    Actor_Apply93(9, 3);
    Actor_Place221(9, 0, 10);
    Actor_Place222(26, k8000_7, 10);
    Actor_Apply94(26, 3);
    Actor_Do81(20);
    Actor_Apply95(9, 1);
    Actor_Place223(9, 0, 10);
    Actor_Run317();
    Actor_Do82(v83a_2);
    Actor_Run318();
}

/* Sets up records 26, 24, 25, 9 and 10 (position/speed, facing, movement),
 * runs several timed particle/object sequences against constant tables, then
 * takes two branches whose outcome picks entries out of the gVal5
 * and gVal6 byte tables to drive further record 9/22 setup calls. */
void RunEventScript01(void)
{
    u32 i;
    s32 entry;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    ObjectMotion_ArmCallback_1(26, 0x3000, 0);
    ObjectMotion_ArmCallback_2(24, 0xd000, 0);
    ObjectMotion_ArmCallback_3(25, 0xb000, 0);
    Motion_ArmCb_4(9, 0x3000, 0);
    Motion_ArmCb_5(10, 0xd000, 20);
    Object_SetModeById_1(26, 3);
    Object_SetModeById_2(24, 3);
    Object_SetModeById_3(25, 3);
    Object_SetModeById_4(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(25, 3);
    BattleRuntime_WaitIfModeZero_1(20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x860000, -1, 0x4ab0000, 1);
    ObjectMotion_SetSpeedParameters_1(26, 0x19999, 0xcccc);
    Motion_SetSpeed_2(9, 0x19999, 0xcccc);
    Motion_EnableActCb_1(26, 0x200cab4);
    Motion_MarkActiveAndSetActionCallback_1(9, 0x200ca78);
    Audio_PlayCue_1(158);
    Actor_Place92(0x200d7a0, 38, 72);
    Battle_WaitMode0_2(10);
    ObjectMotion_SetPositionAndReset_1(9, 149, 0x497);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(9, 0, 0);
    ObjectMotion_SetPositionAndReset_2(25, 250, 0x4be);
    BattleFx_PlayQueuedSound_1();
    Motion_ArmCb_6(10, 0x3000, 0);
    Motion_ArmCb_7(24, 0x3000, 0);
    Motion_ArmCb_8(25, 0x3000, 0);
    Object_SetModeById_5(10, 5);
    Object_SetModeById_6(24, 6);
    Object_SetModeById_7(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = Scene_GetRecord_1(10);
    record = Actor_Check85();
    *(u16 *)(entry + 100) = (Actor_Check86(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Actor_Check87();
    *(u16 *)(entry + 100) = (Actor_Check88(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Actor_Check89();
    *(u16 *)(entry + 100) = (Actor_Check90(record, 90) + 60);
    base5_200cec8 = (s32)gOv9;
    Motion_EnableActCb_2(10, base5_200cec8);
    Motion_EnableActCb_3(24, base5_200cec8);
    Motion_EnableActCb_4(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    Battle_WaitMode0_3(10);
    Audio_PlayCue_2(159);
    Actor_Place93(0x200d7e2, 38, 72);
    Battle_WaitMode0_4(30);
    BattleFx_PlayQueuedSound_2();
    ObjectMotion_PlaceWithinCameraBounds_2(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue_3(158);
    Actor_Place94(0x200d78a, 35, 73);
    Battle_WaitMode0_5(20);
    BattleFx_PlayQueuedSound_3();
    Motion_EnableActCb_5(9, 0x200cb28);
    Battle_WaitMode0_6(20);
    Motion_EnableActCb_6(26, 0x200cb9c);
    Battle_WaitMode0_7(40);
    Audio_PlayCue_4(159);
    Actor_Place95(0x200d7cc, 35, 73);
    Object_LookupAndStep_2(26);
    BattleFx_PlayQueuedSound_4();
    Battle_WaitMode0_8(40);
    base5_e9b = (s32)gVal5;
    SceneWork_SetStepValue_1(base5_e9b);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2(26, 3);
    BattleRuntime_RunThenWaitIfModeZero_2(0x201a, 0, 40);
    Object_SetModeById_8(9, 3);
    Motion_CallWaitAnim_3(26, 3);
    Battle_WaitMode0_9(30);
    Motion_EnableActCb_7(9, 0x200cc0c);
    Motion_EnableActCb_8(26, 0x200cc5c);
    Battle_WaitMode0_10(40);
    Motion_SetSpeedLim_2(0x20000, 0x4000);
    Motion_CamBounds_3(0x690000, -1, 0x43e0000, 1);
    Object_LookupAndStep_3(9);
    Motion_ArmCb_9(9, 0, 0);
    BattleFx_SpawnLinked_1(9, 0x100, 40);
    Battle_RunThenWaitIfModeZero_3(9, 0, 10);
    Motion_ArmCb_10(0, 0x8000, 0);
    Motion_ArmCb_11(22, 0x8000, 10);
    ObjectMotion_SetPositionAndReset_3(9, 105, 0x43e);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    Motion_SetSpeed_3(0x8009, 0);
    Motion_ArmCb_12(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the gVal5
     * table to the corresponding follow-up call. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Motion_CallWaitAnim_4(9, 3);
        SceneWork_SetStepValue_2((base5_e9b + 4));
    } else {
        Motion_SetVarCbObj_2(9, 2);
        SceneWork_SetStepValue_3((base5_e9b + 5));
    }
    BattleEvent_RunActionAndWait_1(0x8009, 0);
    Motion_ArmCb_13(22, 0x8000, 40);
    BattleFx_SpawnLinked_2(9, 0x100, 30);
    base5_ea1 = (s32)gVal6;
    SceneWork_SetStepValue_4(base5_ea1);
    Motion_SetSpeed_4(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * gVal6 table and runs its own follow-up sequence. */
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        Motion_CallWaitAnim_5(9, 3);
        SceneWork_SetStepValue_5((base5_ea1 + 1));
        Battle_RunThenWaitIfModeZero_4(0x8009, 0, 30);
        Motion_ArmCb_14(22, 0x8000, 20);
        Object_SetModeById_9(0, 3);
        Object_SetModeById_10(22, 3);
        Motion_CallWaitAnim_6(9, 3);
        Battle_WaitMode0_11(40);
    } else {
        BattleFx_SpawnLinked_3(9, 0x105, 90);
        BattleFx_SpawnLinked_4(9, 0x103, 40);
        Object_SetModeById_11(9, 4);
        SceneWork_SetStepValue_6((base5_ea1 + 2));
        BattleEv_RunWait_2(0x8009, 0);
    }
    Motion_EnableActCb_9(9, 0x200cca8);
    Battle_WaitMode0_12(90);
    Object_LinkPair_1(0, 22, 0);
    Battle_WaitMode0_13(40);
    Object_SetModeById_12(0, 3);
    Motion_CallWaitAnim_7(22, 3);
    Battle_WaitMode0_14(20);
    Object_SetModeById_13(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_1(22);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(22, 0, 0);
}

void Dialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    s32 msg1 = 0xEA5;
    s32 msg0 = 0xEA4;
    u8 *tbl;

    Actor_Run319();
    Actor_Apply96(10, 1);
    Actor_Do83(10);
    Actor_Place224(10, 0, 20);
    if (Actor_Check91((s32)&Value_0000030d) != 0) {
        Actor_Do84(msg1);
        Actor_Place225(10, 0, 10);
    } else {
        Actor_Do85(msg0);
        Actor_Apply97(10, 1);
        Actor_Place226(10, 0, 10);
        Actor_Apply98(10, 2);
        Actor_Place227(10, 0, 10);
    }
    Actor_Place228(10, v2000, 20);
    Actor_Apply99(10, 5);
    Actor_Do86(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_Run65(10);
        v = Actor_Apply100(Actor_Check92(), 0x5A) + 60;
        tbl = gOv9;
        *(u16 *)(rec + 0x64) = v;
        Actor_Apply101(10, tbl);
    }
    Actor_Do87(20);
    Actor_Do88((s32)&Value_0000030d);
    Actor_Run320();
}

void Scene_RunScene372(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 record;

    Actor_Run321(20);
    Actor_Do21(0x166);
    Actor_Run322(0);
    Actor_Run323(1);
    Actor_Run324(2);
    Actor_Run325(3);
    Actor_Run326(4);
    Actor_Run327(5);
    Actor_Run20(0x10003, 1);
    Actor_Run21(0x10000, 2);
    Actor_Run328(1);
    Actor_Run329(120);
    Actor_Run330(0, 0);
    Actor_Run331(60);
    Actor_Run332(60);
    Actor_Do22(0x166);
    Actor_Run333(0);
    Actor_Run334(1);
    Actor_Run335(2);
    Actor_Run336(3);
    Actor_Run337(4);
    Actor_Run338(5);
}

void ActorDraw_SelectActorTwentySevenState(void)
{
    extern s32 gOv12;

    struct Actor *actor = Actor_Run52(27);
    u32 flags = gIw;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void Scene_RunScene372(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Actor_Check30(0);
    rec7 = Actor_Check31(8);
    Actor_Run339();
    if (Actor_Check32(0x305) != 0) {
        Actor_Run340(8);
        Actor_Run341(10);
        Actor_Run342(8, 2);
        Actor_Run343(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_Run344(8, 7);
        } else {
            Actor_Run345(8, 8);
        }
        Actor_Run346(8, 2);
        Actor_Run347(20);
        Actor_Do23(0xed2);
        Actor_Run348(8, 0);
        Actor_Check33(8, 0x200cec8);
        Actor_Run349(8, 6);
    } else {
        Actor_Run350(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Actor_Place96(8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_Run351(8, 7);
        } else {
            Actor_Run352(8, 8);
        }
        Actor_Run353(20);
        Actor_Do24(0xed1);
        Actor_Run354(8, 0, 20);
        Actor_Run355(8, 1);
        Actor_Run356(8, 4, 0);
        Actor_Run357(80);
        Actor_Run358(8, 2);
        Actor_Run359(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_Run360(8, 7);
        } else {
            Actor_Run361(8, 8);
        }
        Actor_Run362(2);
        Actor_Run363(8, 2, 0);
        Actor_Run364(60);
        Actor_Run365(8, 2);
        Actor_Run366(20);
        Actor_Run367(8, 0);
        Actor_Check34(8, 0x200cec8);
        Actor_Run368(8, 6);
        Actor_Do25(0x305);
    }
    Actor_Run369();
}

void Scene_ConfigureActorTwentyTwoScene(void)
{
    extern u8 gOv12[];

    u32 i;
    u8 *record;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Motion_EnableReset_1(ACTOR_ID);
    Actor_Do26(0x200c5b9);
    Motion_SetPosReset_1(0, 0x1e0, 0x570);
    Motion_SetHPosTerrain_1(0, 0, 0);
    Motion_ArmCb_1(ACTOR_ID, 0x3000, 20);
    {
        /* Set bit 0 of the flag byte at +35. */
        u8 *record = Scene_GetRecord_1(ACTOR_ID);
        u8 flags = *(volatile u8 *)&record[35];

        record[35] = (u8)(flags | 1);
    }
    Motion_SetHPosTerrain_2(ACTOR_ID, 0xf90000, 0x4d80000);
    Actor_Run370(1);
    SceneWork_SetStepValue_1(0xed3);
    BattleEv_RunWait_1(0x1016, 0);
    Motion_SetHPosTerrain_3(ACTOR_ID, 0xac0000, 0x4fe0000);
    Actor_Run371(1);
    Motion_SetSpeedLim_1(0x40000, 0x8000);
    Motion_CamBounds_2(0xa20000, 0, 0x5050000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_1(40);
    Motion_CallWaitAnim_1(ACTOR_ID, 4);
    Battle_RunThenWaitIfModeZero_1(0x1016, 0, 10);
    Motion_ArmCb_2(ACTOR_ID, 0xc000, 20);
    Motion_SetVarCbObj_1(ACTOR_ID, 2);
    Battle_RunThenWaitIfModeZero_2(0x1016, 0, 10);
    Motion_ArmCb_3(ACTOR_ID, 0x1000, 20);
    Motion_CallWaitAnim_2(ACTOR_ID, 3);
    Motion_SetSpeed_1(ACTOR_ID, 0x20000, 0x10000);
    Motion_SetPosReset_2(ACTOR_ID, 165, 0x514);
    Motion_SetPosReset_3(ACTOR_ID, 195, 0x598);
    GameFlag_Set_1(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void Scene_BuildPlacementGrid(void)
{
    extern u8 gOv12[];

    u32 i;
    u8 *record;

    Actor_SetRect6(16, 96, 11, 73, 6, 3); /* main:08009180 */
    Actor_SetRect7(16, 96, 34, 68, 14, 10); /* main:08009180 */
    Actor_SetRect8(16, 96, 64, 68, 7, 7); /* main:08009180 */
    Actor_SetRect9(9, 95, 11, 73, 6, 3); /* main:08009180 */
    Actor_SetRect10(40, 94, 34, 68, 14, 10); /* main:08009180 */
    Actor_SetRect11(54, 94, 64, 68, 8, 7); /* main:08009180 */
    Actor_SetRect12(72, 75, 72, 76, 1, 1); /* main:08009180 */
    Actor_SetRect13(72, 75, 74, 76, 1, 1); /* main:08009180 */
    Actor_SetRect14(7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Actor_SetRect15(8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Actor_SetRect16(8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Actor_SetRect17(8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Actor_SetRect18(11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Actor_SetRect19(12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Actor_SetRect20(25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Actor_Run372(); /* main:08009128 */
}

void State_SetWords1c0And1c8AndRun(void)
{
    void Actor_Run66(s32);

    u8 *state;

    Actor_Run373();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Actor_Do89(0x87c);
    Actor_Apply102(12, 2);
    Actor_Do90(0x900);
    Actor_Run374();
}

void State_SetWorkWordsAndFlag87f(void)
{
    void Actor_Run66(s32);

    u8 *state;

    Actor_Run375();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Actor_Do91(0x87f);
    Actor_Apply103(12, 3);
    Actor_Do92(0x900);
    Actor_Run376();
}

void Actor_SetModeByFrameBit1(s32 o)
{
    extern s32 gOv12;

    s32 v;

    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Actor_Apply104(o, 7);
    } else {
        Actor_Apply105(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)0x03001e40;
        v = (*(s32 *)0x0200d7fc << 3) + 16;
        if (Actor_Apply106(*q, v) == 0) {
            Actor_Do93(o);
        }
    }
}

void OvObj_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)0x03001e40;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Actor_Apply107(obj, Actor_Apply108(t, 6));
    }
    n = (*(s32 *)0x0200d7fc << 3) + 16;
    if (Actor_Apply109(*fc, n) == 0) {
        Actor_Do94(obj);
    }
}

void OvObj_ApplyIwramWord1e40(s32 o)
{
    volatile s32 *p = (s32 *)0x03001e40;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Actor_Apply110(o, Actor_Apply111(t, 6));
    }
}

void Effect_UpdateArcOverAnchor(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Actor_Do95(o);
    } else {
        d = Actor_Check93(t << 10);
        o->f18 = d;
        o->f1c = d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 + ((k << 2) + k) + 0x80000;
    }
}

void OvObj_UpdateArcFromParent(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Actor_Do96(o);
    } else {
        d = Actor_Check94(t << 10);
        o->f18 = d;
        o->f1c = -d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 - ((k << 2) + k) + 0x100000;
    }
}

void State_SetValue140Mode0(void)
{
    void Actor_Run66(s32);

    Actor_Apply112(140, 0);
}

void Scene_RunSingleStep(void)
{
    Actor_Run377();
}

void Scene_RunFourPairedSteps(void)
{
    extern u8 gOv12[];

    Actor_Do97(32);
    Actor_Run378();
    Actor_Do98(33);
    Actor_Run379();
    Actor_Do99(30);
    Actor_Run380();
    if (*(s32 *)0x0200d7f8 == 0) {
        Actor_Do100(29);
        Actor_Run381();
    }
}

void State_SetValue19ThenCall(void)
{
    void Actor_Run66(s32);

    Actor_Do101(19);
    Actor_Run382();
}

void OvObj_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = Actor_Run22(0)->f50;
    dst = Actor_Run23(22)->f50;
    dst->f = src->f;
    dst2 = Actor_Run24(8)->f50;
    dst2->f = src->f;
}

void State_SetValueEe4(void)
{
    void Actor_Run66(s32);

    Actor_Run383();
    Actor_Apply113(0xee4, 1);
    Actor_Run384();
}

void Scene_RunScriptedStep1120(void)
{
    extern u8 gOv12[];

    Actor_Run385();
    Actor_Apply114((s32)&Value_00001120, 1);
    Actor_Run386();
}
