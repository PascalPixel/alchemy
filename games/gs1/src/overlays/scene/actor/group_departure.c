#include "types.h"
#include "scene.h"

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

Rec *Actor_unk22_4(s32);
Rec *Actor_unk23_4(s32);
Rec *Actor_unk24_4(s32);

u8 *Actor_unk25_4(s32);

u8 *Actor_unk26_4(s32);
u8 *Actor_unk27_4(s32);
u8 *Actor_unk28_4(s32);
u8 *Actor_unk29_4(s32);
u8 *Actor_unk30_4(s32);
u8 *Actor_unk31_4(s32);
u8 *Actor_unk32_4(s32);
u8 *Actor_unk33_4(s32);

u8 *Actor_unk34_4(s32);

u8 *Actor_unk35_4(s32);

u8 *Actor_unk36_4(s32);

u8 *Actor_unk37_4(s32);

u8 *Actor_unk38_4(s32);

u8 *Actor_unk39_4(s32);
u8 *Actor_unk40_4(s32);
u8 *Actor_unk41_4(s32);

u8 *Actor_unk42_4(s32);

u8 *Actor_unk43_4(s32);

u8 *Actor_unk44_4(s32);

u8 *Actor_unk45_4(s32);

u8 *Actor_unk46_4();

u8 *Actor_unk47_4();

u8 *Actor_unk48_4();

u8 *Actor_unk49_4();

Thing1 *Actor_unk50_4(s32);

Thing2 *Actor_unk51_4(s32);

struct Actor *Actor_unk52_4(s32);

u8 *Actor_unk53_4(s32);

u8 *Actor_unk54_4(s32);

u8 *Actor_unk55_4(s32);

u8 *Actor_unk56_4(s32);

u8 *Actor_unk57_4(s32);

u8 *Actor_unk58_4(s32);

u8 *Actor_unk59_4(s32);

u8 *Actor_unk60_4(s32);

u8 *Actor_unk61_4(s32);
u8 *Actor_unk62_4(s32);

u8 *Actor_unk63_4(s32);
u8 *Actor_unk64_4(s32);

u8 *Scene_GetRecord_1();

u8 *Actor_unk65_4(s32);

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

s32 OvObj_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Actor_unk35();
        counter = Actor_Apply(Actor_unk36(), 20) + 20;
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

    Actor_unk27_2(0x210);
    a = 10;
    b = 84;
    Actor_unk21_5(40, 84, 7, 4, a, b);
}

void State_SetFlag210AndConfigureRegion40_89(void)
{
    void Actor_unk66_4(s32);

    s32 a;
    s32 b;

    Actor_unk28_2(0x210);
    a = 10;
    b = 84;
    Actor_unk22_5(40, 89, 7, 4, a, b);
}

void State_SetWork1c0AndRunObject(u8 *o)
{
    void Actor_unk66_4(s32);

    u8 *state;

    if (Actor_unk37((s32)&Value_00000834) != 0) {
        Actor_unk67_4();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Actor_unk29_2(o);
}

void Scene_SetupDescriptorD774(void)
{
    Actor_unk30_2(0x9E);
    Actor_unk97_3(gOv6, 45, 11);
    Actor_unk98_3(0, (s32)&Value_00000101, 0x1A4);
    Actor_unk31_2(3);
    Actor_unk32_2(11);
}

void State_SetValue123Mode1(void)
{
    void Actor_unk66_4(s32);

    Actor_unk33_2(0x7B);
    Actor_unk34_2(1);
}

void State_ApplyValues123And3(void)
{
    void Actor_unk66_4(s32);

    Actor_unk35_2(0x7B);
    Actor_unk36_2(3);
}

void State_SetValue123Mode4(void)
{
    void Actor_unk66_4(s32);

    Actor_unk37_2(0x7B);
    Actor_unk66_4(4);
}

void Scene_RunStep7BAndCheckFlags841And842(void)
{
    Actor_unk38_2(0x7B);
    if (Actor_unk38((s32)&Value_00000841) != 0
        && Actor_unk39((s32)&Value_00000842) == 0) {
        Actor_unk68_4();
    }
    Actor_unk39_2(2);
}

void Scene_SetupDescriptorD78a(void)
{
    Actor_unk40_2(0x9E);
    Actor_unk99_3(gOv7, 54, 32);
    Actor_unk100_3(0, 0x196, (s32)&Value_000002d7);
    Actor_unk41_2(3);
    Actor_unk42_2(5);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    if (Actor_Check(0x206) == 0) {
        Actor_unk69_4(158);
        Actor_Place(0x200d7a0, 45, 39);
    }
    if (Actor_unk2(0x835) == 0) {
        record = Actor_unk3(0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        Actor_unk70_4();
        Actor_Do(0x206);
    } else {
        L_020002b4:;
        Actor_unk2_3(0, 0x106, 0x325);
        Actor_unk71_4(3);
        Actor_unk72_4(6);
    }
}

void Scene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (Actor_unk40((s32)&Value_00000205) == 0) {
        Actor_unk43_2(0x9E);
        Actor_unk101_3(gOv7, 50, 44);
    }
    Actor_unk102_3(0, 0x154, 0x378);
    Actor_unk44_2(3);
    Actor_unk45_2(7);
}

void Scene_SetupWithDescriptorD7A0(void)
{
    Actor_unk46_2(158);
    Actor_unk103_3((void *)0x0200d7a0, 49, 69);
    Actor_unk104_3(0, 0x146, 0x466);
    Actor_unk47_2(3);
    Actor_unk48_2(8);
}

void Scene_SetupDescriptorD7b6(void)
{
    Actor_unk49_2(158);
    Actor_unk105_3((void *)0x0200d7b6, 52, 76);
    Actor_unk106_3(0, 0x176, 0x4d6);
    Actor_unk50_2(3);
    Actor_unk51_2(9);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_unk73_4(158);
    Actor_unk3_3(0x200d78a, 35, 74);
    Actor_unk4_3(0, 102, 0x4b6);
    Actor_unk74_4(3);
    Actor_unk75_4(10);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_unk76_4(158);
    Actor_unk5_3(0x200d78a, 35, 73);
    Actor_unk6_3(0, 102, 0x4b6);
    Actor_unk77_4(3);
    Actor_unk78_4(12);
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_unk79_4(158);
    Actor_unk7_3(0x200d7a0, 38, 72);
    Actor_unk8_3(0, 146, 0x49e);
    Actor_unk80_4(3);
    Actor_unk81_4(13);
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

    Actor_unk52_2(0xAA);
    Actor_unk107_3(23, 0, 0);
    if (Actor_unk41((s32)&Value_00000109) != 0) {
        Actor_unk53_2((s32)&Value_00000205);
        Actor_unk54_2((s32)&Value_00000206);
    }
    if (Actor_unk42(0x830) != 0) {
        Actor_unk108_3(11, w1, w2);
        Actor_unk82_4();
    }
    if (Actor_unk43((s32)&Value_00000831) != 0) {
        Actor_unk109_3(12, w3, w4);
        Actor_unk83_4();
    }
    if (Actor_unk44((s32)&Value_00000832) != 0) {
        Actor_unk110_3(13, w5, p1);
        Actor_unk84_4();
    }
    if (Actor_unk45((s32)&Value_00000833) != 0) {
        Actor_unk111_3(14, w6, p2);
        Actor_unk85_4();
    }
    {
        u8 *q;
        q = Actor_unk26_4(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Actor_unk27_4(12);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk28_4(13);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk29_4(14);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk30_4(15);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk31_4(16);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk25_4(17);
        q += 0x59;
        *q = *q | m;
        q = Actor_unk32_4(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (Actor_unk46((s32)&Value_00000837) != 0) {
        Actor_unk112_3(22, 0, 0);
    }
    {
        u8 *q;
        q = Actor_unk34_4(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (Actor_unk47((s32)&Value_00000838) != 0) {
        Actor_unk113_3(19, w7, p3);
    } else {
        Actor_Apply2(19, gOv8);
    }
    if (Actor_unk33_4((s32)&Value_00000841) != 0) {
        s32 h2;
        u8 *tbl;
        Actor_unk86_4();
        Actor_unk114_3(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Actor_unk35_4(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply3(Actor_unk48(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = gOv9;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Actor_Apply4(9, tbl);
        }
        Actor_unk115_3(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Actor_unk36_4(26);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply5(Actor_unk49(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Actor_Apply6(26, tbl);
        }
        Actor_unk116_3(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Actor_unk37_4(22);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply7(Actor_unk50(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Actor_Apply8(22, tbl);
        }
        Actor_unk117_3(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Actor_unk38_4(8);
            *(u16 *)(o + 6) = h2;
            v = Actor_Apply9(Actor_unk51(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Actor_Apply10(8, tbl);
        }
        Actor_Apply11(8, 6);
        {
            u8 *r;
            r = Actor_unk39_4(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Actor_unk40_4(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Actor_Apply12(gOv10, 0xC80);
        Actor_unk118_3(24, 0, 0);
        Actor_unk119_3(25, 0, 0);
        Actor_unk120_3(23, 0, 0);
        Actor_unk121_3(19, 0, 0);
        if (Actor_unk52((s32)&Value_00000842) != 0) {
            Actor_unk122_3(22, 0, 0);
        }
    } else if (Actor_unk41_4((s32)&Value_0000083a) != 0) {
        u8 *tbl;
        Actor_unk123_3(10, w8, b1);
        Actor_unk124_3(10, w9, 0);
        Actor_Apply13(10, 5);
        {
            u8 *o;
            s32 v;
            o = Actor_unk42_4(10);
            v = Actor_Apply14(Actor_unk53(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = gOv9;
        Actor_Apply15(10, tbl);
        Actor_unk125_3(24, w10, b2);
        Actor_unk126_3(24, w11, 0);
        Actor_Apply16(24, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_unk43_4(24);
            v = Actor_Apply17(Actor_unk54(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_Apply18(24, tbl);
        Actor_unk127_3(25, w12, b3);
        Actor_unk128_3(25, w13, 0);
        Actor_Apply19(25, 6);
        {
            u8 *o;
            s32 v;
            o = Actor_unk44_4(25);
            v = Actor_Apply20(Actor_unk55(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Actor_Apply21(25, tbl);
        Actor_unk129_3(23, w14, p4);
        Actor_unk130_3(23, 0xC000, 0);
        Actor_Apply22(Actor_unk56(23), 0);
        Actor_unk131_2(17, 0, 0);
        Actor_unk122_3(18, 0, 0);
    } else {
        Actor_unk132_2(17, 0, 0);
        Actor_unk133_2(18, 0, 0);
    }
    {
        s16 *table = (s16 *)gCell;
        if (table[225] != 15 || Actor_unk57((s32)&Value_0000087b) != 0) {
            Actor_unk87_4();
            Actor_unk88_4();
        }
    }
    if (Actor_unk58(0x210) != 0) {
        Actor_unk89_4();
    }
    Actor_unk59((s32)&Value_00000834);
    k = 46;
    Actor_unk23_5(29, 24, 1, 2, 26, k);
    Actor_unk24_5(29, 25, 1, 1, 27, k);
    Actor_unk25_5(29, 25, 1, 1, 28, k);
    k = 20;
    Actor_unk26_5(19, 0x5A, 1, 1, k, 0x58);
    Actor_unk27_5(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Actor_unk45_4(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Actor_Apply23(o, 0);
    }
    Actor_unk60(1);
    if (Actor_unk61((s32)&Value_0000087b) == 0) {
        s16 *table = (s16 *)gCell;
        if (table[225] == 15) {
            Actor_unk90_4();
            return 0;
        }
    }
    Actor_Apply24(23, 7);
    if (Actor_unk62((s32)&Value_00000837) == 0) {
        Actor_unk91_4();
        Actor_Apply25(22, c2);
        Actor_unk134_2(22, w15, p5);
        Actor_unk135_2(21, w16, p6);
        Actor_unk136_2(22, w17, c1);
        Actor_unk137_2(21, w18, 0x26B);
        Actor_Apply26(21, 2);
        Actor_Apply27(22, 5);
        Actor_unk92_4();
    } else {
        Actor_unk93_4();
        Actor_unk138_2(21, w19, p7);
        Actor_unk139_2(21, w20, c3);
        Actor_Apply28(21, 3);
        Actor_unk94_4();
    }
    {
        u8 *state = *(u8 **)0x03001EBC;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Actor_unk95_4();
    Actor_unk96_4();
    Actor_unk97_4();
    return 0;
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_e5c;

    Actor_unk98_4();
    Actor_unk99_4();
    Actor_unk100_4();
    Actor_unk101_4();
    Actor_unk102_4(60);
    Actor_Run(0x4000, 0x800);
    Actor_unk2_4(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_unk9_3(10, 0x1260000, 0x3640000);
    Actor_unk103_4(0, 0, 0);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    Actor_unk104_4();
    Actor_unk105_4();
    Actor_unk106_4();
    Actor_unk107_4(158);
    Actor_unk10_3(0x200d78a, 50, 44);
    Actor_unk3_4(22, 0x101);
    Actor_unk11_3(9, 0xcccc, 0x6666);
    Actor_unk12_3(0, 0xcccc, 0x6666);
    Actor_unk13_3(10, 0xcccc, 0x6666);
    Actor_unk14_3(9, 0x1560000, 0x37a0000);
    Actor_unk15_3(9, 0x156, 0x389);
    Actor_unk108_4();
    Actor_unk16_3(9, 0x128, 0x389);
    Actor_unk17_3(0, 0x1560000, 0x37a0000);
    Actor_unk18_3(0, 0x156, 0x37a);
    Actor_unk19_3(0, 0x156, 0x389);
    Actor_unk20_3(0, 0x13e, 0x389);
    Actor_unk109_4(9, 1);
    Actor_unk110_4(9, 1);
    Actor_unk21_3(9, 0xc000, 60);
    base5_e5c = (s32)gVal;
    Actor_unk111_4(base5_e5c);
    Actor_unk112_4(9, 0);
    Actor_unk22_3(10, 0x126, 0x346);
    Actor_unk113_4(40);
    Actor_unk114_4(10, 4);
    Actor_unk115_4(10, 0);
    Actor_unk116_4(0, 9, 0);
    Actor_unk117_4(40);
    Actor_unk23_3(10, 0x4000, 0);
    Actor_unk118_4(10, 0, 20);
    Actor_unk24_3(9, 0x101, 20);
    Actor_unk25_3(9, 0xc000, 10);
    Actor_unk119_4(9, 0, 10);
    Actor_unk120_4(10, 4);
    Actor_unk121_4(10, 0);
    Actor_unk4_4(9, 0x102);
    Actor_unk122_4(30);
    Actor_unk123_4(9, 0, 50);
    Actor_unk26_3(9, 0xc000, 10);
    Actor_unk27_3(9, 0x18000, 0xc000);
    Actor_unk28_3(9, 0x121, 0x373);
    Actor_unk29_3(9, 0xe000, 0);
    Actor_unk124_4(9, 0);
    Actor_unk125_4(10, 2);
    Actor_unk126_4(10, 0);
    Actor_unk127_4(9, 4);
    Actor_unk128_4(9, 0, 10);
    Actor_unk30_3(9, 0x2000, 10);
    Actor_unk129_4((base5_e5c + 8));
    Actor_unk4(9, 0);
    Actor_unk31_3(0, 0x12e, 0x389);
    Actor_unk32_3(0, 0xc000, 0);
    while (Actor_unk5(0, 0) == 1) {
        Actor_unk130_4(9, 1);
        Actor_unk2_2(0xe65);
        Actor_unk131_3(9, 0);
    }
    Actor_unk132_3(9, 3);
    Actor_unk3_2(0xe66);
    Actor_unk133_3(9, 0, 10);
    Actor_unk134_3(0, 3);
    Actor_unk33_3(10, 0x18000, 0xc000);
    Actor_unk34_3(10, 0x129, 0x2ee);
    Actor_unk135_3(10);
    Actor_unk35_3(9, 0x129, 0x2ee);
    Actor_unk136_3(9, 0, 0);
    Actor_unk137_3(10, 0, 0);
    Actor_unk138_3(10, 1);
    Actor_unk139_3(21, 2);
    Actor_unk140_3(22, 5);
    Actor_unk4_2(0x12f);
    Actor_unk5_2(0x87b);
    Actor_unk6_2(0x205);
    Actor_unk141_3();
}

void Scene_RunScene372(void)
{
    u32 i;
    s32 record;

    Actor_SetRect(29, 64, 1, 1, 21, 57);
    Actor_unk2_5(29, 64, 1, 1, 21, 58);
    Actor_unk3_5(29, 64, 1, 1, 22, 58);
    Actor_unk4_5(29, 64, 1, 1, 20, 58);
    Actor_unk5_5(28, 20, 1, 1, 20, 57);
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

    Actor_unk28_5(29, 23, 1, 1, a, d);
    b = 14;
    Actor_unk29_5(29, 23, 1, 1, b, d);
    c = 13;
    Actor_unk30_5(29, 23, 1, 1, c, d);
    Actor_unk31_5(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Actor_unk32_5(25, 21, 1, 1, c, e);
    Actor_unk33_5(25, 21, 1, 1, a, e);
    Actor_unk34_5(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Actor_unk35_5(13, 0x37, 1, 1, a, f);
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

    if (Actor_unk6(0x312) != 0) {
        v5 = r8;
    } else {
        Actor_unk142_3();
        v5 = r8;
        if (Actor_unk7(0x832) == 0) {
            rec8 = Actor_unk8(13);
            record = Actor_unk46_4(0);
            p5 = (u8 *)(s32)*(volatile u8 *)(*(volatile s32 *)((s32)record + 80) + 9);
            record += 35;
            p10 = *record;
            Actor_unk36_3(0x40000, 0x40000, 0x10000);
            Actor_unk143_3(141);
            Actor_unk144_3(40);
            Actor_unk145_3(145);
            Actor_unk146_3(0, 3);
            {
                u8 *record = Actor_unk47_4(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
            Actor_unk37_3(13, 0, 0x2bf0000);
            *(volatile s32 *)(rec8 + 48) = 0x18000;
            *(volatile s32 *)(rec8 + 52) = 0x18000;
            {
                s32 shifted = *(volatile s32 *)(rec8 + 12) + 0x500000;

                *(volatile s32 *)(rec8 + 12) = shifted;
                *(volatile s32 *)(rec8 + 60) = shifted;
            }
            *(volatile s32 *)(rec8 + 68) = 0x8000;
            Actor_unk38_3(13, 64, 0x2bf);
            Actor_unk147_3(40);
            Actor_unk7_2(0x121);
            Actor_unk39_3(-1, -1, 0xe666);
            Actor_unk148_3();
            Actor_unk149_3();
            Actor_unk8_2(0x832);
            Actor_unk150_3(0, ((u32)(s32)((s32)p5 << 28) >> 30));
            {
                u8 *record = Actor_unk48_4(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 1);
            }
            *record = p10;
        }
        Actor_unk151_3();
        Actor_unk9_2(0x312);
        if (Actor_unk9(0x837) != 0) {
            if (Actor_unk10(0x841) == 0) {
                if (Actor_unk11(0x30c) == 0) {
                    later = Actor_unk49_4(0);
                    if (*(volatile s32 *)((s32)later + 16) <= 0x2b4ffff) {
                        Actor_unk12(62, 0x29d);
                        Actor_unk40_3(0, 27, 0x273);
                    } else {
                        Actor_unk13(75, 0x2cb);
                        Actor_unk41_3(0, 67, 0x2f5);
                    }
                    Actor_unk10_2(0x30c);
                }
            }
        }
        Actor_unk152_3();
    }
    p10b = v5;
}

void State_ApplyFourRects(void)
{
    void Actor_unk66_4(s32);

    s32 a = 0x2a;
    s32 b;

    Actor_unk36_5(29, 22, 1, 1, 3, a);
    b = 2;
    Actor_unk37_5(29, 21, 1, 1, b, a);
    Actor_unk38_5(29, 21, 1, 1, 4, a);
    Actor_unk39_5(23, 20, 3, 1, b, 0x2b);
}

void Scene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Actor_unk40_5(29, 20, 1, 1, a, h);
    b = 0x46;
    Actor_unk41_5(29, 20, 1, 1, a, b);
    a2 = 27;
    Actor_unk42_5(29, 20, 1, 1, a2, b);
    Actor_unk43_5(28, 21, 1, 1, 28, h);
    Actor_unk44_5(28, 22, 1, 1, a2, 0x48);
}

void Scene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Actor_unk153_3();
    Actor_unk42_3(0, 0x106, 0x32a);
    Actor_unk43_3(20, 0x1060000, 0x3250000);
    Actor_unk44_3(20, 0x106, 0x339);
    Actor_unk45_3(0, 0x20000, 0x10000);
    Actor_unk154_3(0, 2, 0);
    Actor_unk46_3(0, 0x11a, 0x357);
    Actor_unk155_2(20, 1);
    Actor_unk156_2(0, 4, 0);
    Actor_unk157_2(0, 20, 0);
    Actor_unk158_2();
    Actor_unk159_2(30);
    Actor_unk160_2(0, 2);
    Actor_unk47_3(20, 0x100, 20);
    base5_e67 = (s32)gVal2;
    Actor_unk161_2(base5_e67);
    Actor_unk162_2(20, 0);
    Actor_unk163_2(20);
    Actor_unk14(20, 0);
    Actor_unk164_2(20, 2);
    Actor_unk165_2((base5_e67 + 4));
    Actor_unk166_2(20, 0, 20);
    Actor_unk5_4(20, 0x200c8c0);
    Actor_unk11_2(0x835);
    Actor_unk167_2();
}

void Scene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (Actor_unk15(0x836) == 0) {
        if (Actor_unk16(0x837) == 0) {
            Actor_unk168_2();
            Actor_unk12_2(0xe6c);
            Actor_unk169_2(22, 0, 20);
            Actor_unk48_3(0, 0x101, 40);
            Actor_unk49_3(0, 0x17e, 0x26b);
            Actor_unk170_2(0, 22, 0);
            Actor_unk171_2(0, 2);
            Actor_unk172_2(30);
            Actor_unk173_2(22, 0);
            Actor_unk13_2(0x836);
            Actor_unk174_2();
        }
    }
}

void Scene_RunActor22SceneWhenFlag836Only(void)
{
    if (Actor_unk63(0x837) == 0 && Actor_unk64(0x836) != 0) {
        Actor_unk175_2();
        Actor_Apply29(22, 2);
        Actor_unk55_2(20);
        Actor_unk56_2(0xe71);
        Actor_unk176_2();
        Actor_unk177_2();
    }
}

void Scene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (Actor_unk17(0x841) != 0) {
        Actor_unk178_2();
        Actor_unk179_2(22, 0, 0);
        Actor_unk180_2(20);
        Actor_unk181_2((s32)gVal3);
        Actor_unk182_2(22, 0);
        Actor_unk50_3(22, 0xe000, 10);
        Actor_unk183_2();
    } else {
        if (Actor_unk18(0x837) == 0) {
            Actor_unk184_2();
            Actor_unk14_2(0xe6e);
            Actor_unk185_2();
            Actor_unk186_2();
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

    if (Actor_unk19(0x837) != 0) {
    } else {
        Actor_unk187_2();
        Actor_unk6_4(22, 0x100);
        base5_e74 = (s32)gVal4;
        Actor_unk188_2(base5_e74);
        Actor_unk189_2(22, 0);
        Actor_unk51_3(0, 0x100, 20);
        Actor_unk52_3(0, 0x4000, 0);
        Actor_unk7_4(0x6666, 0xccc);
        Actor_unk8_4(0x1000000, -1, 0x24c0000, 1);
        Actor_unk53_3(22, 0x20000, 0x10000);
        Actor_unk20(22, 0x200c934);
        Actor_unk190_2(0, 22, 0);
        Actor_unk191_2(30);
        Actor_unk21(22, 0x200c984);
        Actor_unk192_2(22, 0);
        v6 = 128;
        record = Actor_unk65(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Actor_unk193_2(22, 1);
        Actor_unk194_2(20);
        Actor_unk195_2(22, 0);
        Actor_unk196_2(40);
        Actor_unk197_2(22, 1);
        Actor_unk198_2((base5_e74 + 5));
        Actor_unk199_2(22, 0, 20);
        Actor_unk200_2(0, 3);
        Actor_unk201_2(22, 3);
        Actor_unk202_2(22, 0);
        Actor_unk54_3(22, (v6 << 9), 0x8000);
        Actor_unk203_2(22, 2);
        record = Actor_unk22(0);
        if (record != 0) {
            Actor_unk204_2(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_unk205_2(22);
        Actor_unk206_2(22, 0, 0);
        Actor_unk207_2(1, 1);
        Actor_unk208_2(21, 3);
        Actor_unk15_2(0x837);
        Actor_unk209_2();
    }
}

void Scene_RunScene372SequenceD(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 record;

    Actor_unk210_2();
    record = Actor_unk23(0);
    if (record != 0) {
        Actor_unk211_2(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Actor_unk55_3(22, 0x10000, 0x8000);
    Actor_unk56_3(22, 0x119, 0x1fb);
    Actor_unk212_2(22, 0, 0);
    Actor_unk213_2(30);
    Actor_unk16_2(0xe7b);
    Actor_unk214_2(22, 0);
    Actor_unk215_2(0, 22, 0);
    Actor_unk216_2(10);
    Actor_unk217_2(0, 1);
    Actor_unk218_2(20);
    Actor_unk57_3(22, 0x4000, 0);
    Actor_unk219_2(22, 0);
    Actor_unk220_2(22, 2);
    record = Actor_unk24(0);
    if (record != 0) {
        Actor_unk221_2(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_unk222_2(22);
    Actor_unk223_2(22, 0, 0);
    Actor_unk58_3(0, 0x100, 0x205);
    Actor_unk224_2();
}

void Actor_RunActor22PlacementSequence(s32 x, s32 y)
{
    extern s32 gOv12;

    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Actor_unk50_4(0);
    if (a != 0) {
        Actor_unk140_2(22, a->unk8, a->unk10);
    }
    Actor_unk141_2(22, w, h);
    Actor_unk142_2(22, x, y);
    Actor_unk143_2(0, 22, 0);
    Actor_unk57_2(20);
    Actor_Apply30(0, 0x102);
    Actor_unk58_2(40);
    Actor_unk59_2(0xe7d);
    Actor_Apply31(22, 0);
    Actor_Apply32(22, 2);
    Actor_Apply33(22, 0);
    Actor_Apply34(0, 3);
    Actor_Apply35(22, 2);
    b = Actor_unk51_4(0);
    if (b != 0) {
        Actor_unk144_2(22, b->unkA, b->unk12);
    }
    Actor_unk60_2(22);
    Actor_unk145_2(22, 0, 0);
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

    if (Actor_unk25(0x838) != 0) {
        v5 = r9;
        v6 = r10;
        v7 = r8;
    } else {
        Actor_unk225_2();
        Actor_unk17_2(0x200d4b0);
        Actor_unk226_2();
        Actor_unk227_2(1);
        Actor_unk228_2(141);
        Actor_unk59_3(0x20000, 0x20000, 0x10000);
        Actor_unk229_2(30);
        Actor_unk60_3(0x30000, 0x30000, 0x10000);
        Actor_unk230_2(145);
        Actor_unk231_2(30);
        record = Actor_unk26(0);
        if (record != 0) {
            Actor_unk232_2(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Actor_unk61_3(0, 0x20000, 0x10000);
        Actor_unk62_3(22, 0x20000, 0x10000);
        Actor_unk27(0, 0x200ca00);
        Actor_unk9_4(22, 0x200ca3c);
        Actor_unk233_2(0);
        Actor_unk63_3(0, 0x100, 0);
        Actor_unk64_3(22, 0x100, 30);
        Actor_unk65_3(0x40000, 0x40000, 0x10000);
        Actor_unk234_2(145);
        Actor_unk235_2(40);
        Actor_unk66_3(0x50000, 0x50000, 0x10000);
        Actor_unk236_2(145);
        Actor_unk237_2(20);
        Actor_unk10_4(0, 0x102);
        Actor_unk11_4(22, 0x102);
        Actor_unk238_2(40);
        Actor_unk239_2(32, 5);
        Actor_unk240_2(33, 5);
        Actor_unk241_2(30, 8);
        Actor_unk242_2(29, 8);
        record = Actor_unk66(30);
        *(volatile s32 *)(record + 24) = -0x10000;
        Actor_unk243_2(32, 2);
        Actor_unk244_2(33, 2);
        Actor_unk245_2(30, 3);
        Actor_unk246_2(29, 3);
        Actor_unk18_2(0xe7f);
        Actor_unk247_2(28, 0, 20);
        Actor_unk67_3(0, 0xc000, 0);
        Actor_unk68_3(22, 0xc000, 20);
        Actor_unk12_4(0x40000, 0x8000);
        Actor_unk13_4(0x700000, -1, 0x14b0000, 1);
        Actor_unk248_2();
        for (i = 0; i < 40; i++) {
            Actor_unk249_2(Actor_unk67(32));
            Actor_unk250_2(Actor_unk68(33));
            Actor_unk251_2(Actor_unk69(30));
            Actor_unk252_2(Actor_unk70(29));
            Actor_unk253_2(1);
        }
        base5_200d7fc = (s32)gOv12;
        base7_200c56d = (s32)Actor_unk254_2;
        v5 = base5_200d7fc;
        v6 = 0;
        v7 = base7_200c56d;
        *(volatile s32 *)gOv11 = 0;
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_unk28(base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Actor_unk14_4(base_200c5a9, 0xc80);
        Actor_unk255_2(40);
        *(volatile s32 *)base5_200d7fc = 1;
        Actor_unk256_2(30);
        Actor_unk69_3(19, 0x720000, 0x1220000);
        record = Actor_unk71(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Actor_unk70_3(19, 0xcccc, 0x6666);
        Actor_unk257_2(145);
        Actor_unk71_3(19, 114, 0x14d);
        Actor_unk258_2(19, 2);
        Actor_unk72_3(0x20000, 0x20000, 0x10000);
        Actor_unk259_2(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_unk73_3(19, 0x6666, 0x3333);
        Actor_unk74_3(19, 114, 0x12c);
        Actor_unk260_2(19, 2);
        Actor_unk75_3(0x50000, 0x50000, 0x10000);
        Actor_unk261_2(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Actor_unk76_3(19, 0xcccc, 0x6666);
        Actor_unk77_3(19, 114, 0x14d);
        Actor_unk262_2(19, 2);
        Actor_unk78_3(0x20000, 0x20000, 0x10000);
        Actor_unk263_2(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Actor_unk79_3(19, 0x6666, 0x3333);
        Actor_unk80_3(19, 114, 0x12c);
        Actor_unk264_2(19, 2);
        Actor_unk81_3(0x40000, 0x40000, 0x10000);
        Actor_unk265_2(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Actor_unk82_3(19, 0xcccc, 0x6666);
        Actor_unk83_3(19, 114, 0x14d);
        Actor_unk266_2(19, 2);
        Actor_unk84_3(0x20000, 0x20000, 0x10000);
        Actor_unk267_2(145);
        *(volatile s32 *)base5_200d7fc = 1;
        Actor_unk268_2(20);
        Actor_unk15_4(32, 0x102);
        Actor_unk269_2(32, 2);
        Actor_unk270_2(31, 0);
        Actor_unk85_3(33, 0x100, 0);
        Actor_unk271_2(33, 2);
        Actor_unk272_2(28, 0, 40);
        Actor_unk16_4(30, 0x102);
        Actor_unk273(30, 2);
        Actor_unk274(30, 0);
        *(volatile s32 *)gOv11 = 1;
        Actor_unk275(29, 1);
        Actor_unk276(1);
        Actor_unk277(29, 0);
        Actor_unk86_3(29, 0x105, 20);
        Actor_unk87_3(29, 0x8000, 40);
        Actor_unk278(29, 0, 20);
        Actor_unk88_3(29, 0x8000, 20);
        Actor_unk89_3(29, 0x4000, 40);
        Actor_unk90_3(29, 0x100, 0);
        Actor_unk279(29, 2);
        Actor_unk280(29, 4, 40);
        Actor_unk281(29, 9);
        Actor_unk282(10);
        Actor_unk283(29, 0, 20);
        Actor_unk19_2(0x121);
        Actor_unk91_3(-1, -1, 0xe666);
        Actor_unk17_4(0x60000, 0xc000);
        Actor_unk18_4(0x540000, -1, 0x2340000, 1);
        Actor_unk284();
        Actor_unk285();
        Actor_unk286(22, 0, 0);
        Actor_unk287(20);
        Actor_unk19_4(22, 0x102);
        Actor_unk288(30);
        Actor_unk289(base7_200c56d);
        Actor_unk290(base_200c5a9);
        Actor_unk291(22, 0);
        Actor_unk292(0, 22, 0);
        Actor_unk293(20);
        Actor_unk294();
        Actor_unk295(0, 3);
        Actor_unk296(22, 3);
        Actor_unk297(20);
        Actor_unk298(22, 2);
        record = Actor_unk29(0);
        if (record != 0) {
            Actor_unk299(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_unk300(22);
        Actor_unk301(22, 0, 0);
        Actor_unk302(31);
        Actor_unk303(28);
        Actor_unk304(30);
        Actor_unk305(29);
        Actor_unk306(32);
        Actor_unk307(33);
        Actor_unk20_2(0x838);
        Actor_unk308();
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
        Actor_Apply36(Actor_unk72(32), 1);
        Actor_Apply37(Actor_unk73(33), 1);
        Actor_Apply38(Actor_unk74(30), 1);
        Actor_Apply39(Actor_unk75(29), 1);
    } else {
        Actor_Apply40(Actor_unk76(32), 8);
        Actor_Apply41(Actor_unk77(33), 8);
        Actor_Apply42(Actor_unk78(30), 8);
        Actor_Apply43(Actor_unk79(29), 8);
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

    if (Actor_unk80((s32)&Value_0000083a) != 0) {
        return;
    }
    Actor_unk309();
    Actor_unk146_2(10, kc0_1, k4be_1);
    Actor_unk147_2(10, k2000_1, 0);
    Actor_Apply44(10, 5);
    {
        u8 *o;
        s32 v;
        o = Actor_unk53_4(10);
        v = Actor_Apply45(Actor_unk81(), 0x5A) + 60;
        tbl = gOv9;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply46(10, tbl);
    }
    Actor_unk148_2(9, kc0_2, k4a5_1);
    Actor_unk149_2(9, k2000_2, 0);
    Actor_unk150_2(24, ke3_1, k4be_2);
    Actor_unk151_2(24, k4000_1, 0);
    Actor_Apply47(24, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_unk54_4(24);
        v = Actor_Apply48(Actor_unk82(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply49(24, tbl);
    }
    Actor_unk152_2(25, kfa_1, k4be_3);
    Actor_unk153_2(25, k4000_2, 0);
    Actor_Apply50(25, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_unk55_4(25);
        v = Actor_Apply51(Actor_unk83(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_Apply52(25, tbl);
    }
    Actor_unk154_2(26, ke3_2, k4a5_2);
    Actor_unk155(26, k2000_3, 0);
    Actor_unk156(23, kf3_1, k4fd_1);
    Actor_unk157(23, kc000_1, 0);
    Actor_Apply53(Actor_unk84(23), 0);
    Actor_unk61_2(3);
    Actor_unk62_2((s32)&Value_00000e8c);
    Actor_Apply54((s32)&Value_0000201a, 0);
    Actor_unk158(0, k100_1, 20);
    Actor_unk159(0, 150, k446_1);
    {
        u8 *p;
        p = Actor_unk56_4(0);
        if (p != 0) {
            Actor_unk160(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Actor_unk161(22, 132, k446_2);
    Actor_unk162(0, 22, 0);
    Actor_unk63_2(40);
    Actor_unk163(0, k4000_3, 0);
    Actor_unk164(22, k4000_4, 20);
    Actor_Apply55(k40000_1, k8000_1);
    Actor_SetMode(kd8_1, ng1, k4d0_1, 1);
    Actor_unk310();
    Actor_unk64_2(40);
    Actor_Apply56(10, 2);
    Actor_unk165(10, 0, 10);
    Actor_Apply57(23, 3);
    Actor_unk166(9, 0, 10);
    Actor_Apply58(9, 3);
    Actor_unk167(9, 0, 10);
    Actor_unk168(9, k3000_1, 10);
    Actor_Apply59(k30000_1, k6000_1);
    Actor_SetMode2(ke8_1, ng2, k4e5_1, 1);
    Actor_unk311();
    Actor_unk65_2(20);
    Actor_unk66_2(134);
    Actor_unk169(23, 4, 0);
    Actor_Apply60(23, 6);
    Actor_unk67_2(10);
    Actor_unk170(23, 0, 0);
    Actor_unk68_2(60);
    Actor_unk312();
    Actor_Apply61(10, 1);
    {
        u8 *o;
        o = Actor_unk57_4(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_Apply62(24, 1);
    {
        u8 *o;
        o = Actor_unk58_4(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_Apply63(25, 1);
    {
        u8 *o;
        o = Actor_unk59_4(25);
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
    Actor_unk313();
    Actor_Apply70(26, k102_1);
    Actor_Apply71(9, k102_2);
    Actor_unk69_2(60);
    Actor_Apply72(26, 2);
    Actor_Apply73(26, 3);
    Actor_Apply74(26, 0);
    Actor_unk171(25, 2, 0);
    Actor_unk172(25, 234, k4b5_1);
    Actor_unk173(26, 2, 0);
    Actor_unk174(26, 227, k4b1_1);
    Actor_unk70_2(90);
    Actor_SetMode4(ke8_2, ng4, k4e5_2, 1);
    Actor_unk314();
    Actor_unk175(23, kf3_2, k4fd_2);
    Actor_unk71_2(1);
    Actor_unk72_2(106);
    {
        u8 *o;
        o = Actor_unk60_4(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Actor_unk73_2(6);
    Actor_Apply75(23, 7);
    Actor_unk74_2(20);
    Actor_unk315();
    Actor_unk75_2(20);
    Actor_Apply76(k19999_1, k3333_1);
    Actor_SetMode5(kd8_3, ng5, k4d0_3, 1);
    Actor_unk316();
    Actor_Apply77(24, 2);
    Actor_unk76_2(20);
    Actor_unk176(24, k105_1, 40);
    Actor_unk177(24, 10, 0);
    Actor_unk77_2(10);
    Actor_Apply78(10, 2);
    Actor_Apply79(k800a_1, 0);
    {
        u8 *o;
        o = Actor_unk61_4(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_unk62_4(26);
        o += 0x5A;
        *o = *o & m;
    }
    Actor_unk178(25, k9999_2, k4ccc_1);
    Actor_unk179(26, k9999_3, k4ccc_2);
    Actor_unk180(25, 247, k4ba_1);
    Actor_unk181(26, 227, k4a5_3);
    {
        u8 *o;
        o = Actor_unk63_4(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_unk64_4(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Actor_unk182(26, k6000_2, 0);
    Actor_unk183(25, k8000_2, 10);
    Actor_Apply80(24, 4);
    Actor_unk184(k8018_1, 0, 10);
    Actor_unk185(10, kc000_2, 20);
    Actor_unk186(10, 0, 10);
    Actor_Apply81(10, 4);
    Actor_unk187(k800a_2, 0, 10);
    Actor_unk188(24, k105_2, 0);
    Actor_unk189(10, k105_3, 60);
    Actor_unk190(9, k106_1, 20);
    Actor_unk191(9, k8000_3, 40);
    Actor_unk192(9, kc000_3, 20);
    Actor_unk193(9, 0, 30);
    Actor_unk194(9, k4000_5, 10);
    Actor_unk195(9, 0, 10);
    Actor_unk196(10, kc000_4, 0);
    Actor_unk197(25, k9000_1, 0);
    Actor_unk198(24, ka000_1, 0);
    Actor_unk199(26, k8000_4, 10);
    Actor_Apply82(10, 1);
    Actor_unk200(k800a_3, 0, 10);
    Actor_Apply83(9, 4);
    Actor_unk201(9, 0, 10);
    Actor_unk202(10, k105_4, 0);
    Actor_unk203(24, k105_5, 0);
    Actor_unk204(25, k105_6, 0);
    Actor_unk205(26, k105_7, 40);
    Actor_unk206(9, 0, 10);
    Actor_unk207(24, 25, 0);
    Actor_unk78_2(20);
    Actor_unk208(9, 0, 0);
    Actor_unk209(10, 0, 10);
    Actor_unk210(24, k8000_5, 0);
    Actor_unk211(25, k8000_6, 10);
    Actor_Apply84(24, 3);
    Actor_Apply85(25, 3);
    Actor_unk212(10, 9, 0);
    Actor_unk79_2(20);
    Actor_Apply86(10, 1);
    Actor_unk213(k800a_4, 0, 10);
    Actor_Apply87(9, 3);
    Actor_unk214(24, kd000_1, 10);
    Actor_Apply88(24, 1);
    Actor_unk215(24, 0, 10);
    Actor_unk216(10, 0, 0);
    Actor_unk217(9, 0, 0);
    Actor_Apply89(26, 1);
    Actor_unk218(26, k2000_4, 20);
    Actor_unk219(25, ka000_2, 20);
    Actor_Apply90(25, 3);
    Actor_unk220(25, 0, 10);
    Actor_Apply91(26, 3);
    Actor_unk80_2(20);
    Actor_Apply92(9, 2);
    Actor_Apply93(9, 3);
    Actor_unk221(9, 0, 10);
    Actor_unk222(26, k8000_7, 10);
    Actor_Apply94(26, 3);
    Actor_unk81_2(20);
    Actor_Apply95(9, 1);
    Actor_unk223(9, 0, 10);
    Actor_unk317();
    Actor_unk82_2(v83a_2);
    Actor_unk318();
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
    Actor_unk92_3(0x200d7a0, 38, 72);
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
    record = Actor_unk85();
    *(u16 *)(entry + 100) = (Actor_unk86(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Actor_unk87();
    *(u16 *)(entry + 100) = (Actor_unk88(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Actor_unk89();
    *(u16 *)(entry + 100) = (Actor_unk90(record, 90) + 60);
    base5_200cec8 = (s32)gOv9;
    Motion_EnableActCb_2(10, base5_200cec8);
    Motion_EnableActCb_3(24, base5_200cec8);
    Motion_EnableActCb_4(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    Battle_WaitMode0_3(10);
    Audio_PlayCue_2(159);
    Actor_unk93_3(0x200d7e2, 38, 72);
    Battle_WaitMode0_4(30);
    BattleFx_PlayQueuedSound_2();
    ObjectMotion_PlaceWithinCameraBounds_2(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue_3(158);
    Actor_unk94_3(0x200d78a, 35, 73);
    Battle_WaitMode0_5(20);
    BattleFx_PlayQueuedSound_3();
    Motion_EnableActCb_5(9, 0x200cb28);
    Battle_WaitMode0_6(20);
    Motion_EnableActCb_6(26, 0x200cb9c);
    Battle_WaitMode0_7(40);
    Audio_PlayCue_4(159);
    Actor_unk95_3(0x200d7cc, 35, 73);
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

    Actor_unk319();
    Actor_Apply96(10, 1);
    Actor_unk83_2(10);
    Actor_unk224(10, 0, 20);
    if (Actor_unk91((s32)&Value_0000030d) != 0) {
        Actor_unk84_2(msg1);
        Actor_unk225(10, 0, 10);
    } else {
        Actor_unk85_2(msg0);
        Actor_Apply97(10, 1);
        Actor_unk226(10, 0, 10);
        Actor_Apply98(10, 2);
        Actor_unk227(10, 0, 10);
    }
    Actor_unk228(10, v2000, 20);
    Actor_Apply99(10, 5);
    Actor_unk86_2(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_unk65_4(10);
        v = Actor_Apply100(Actor_unk92(), 0x5A) + 60;
        tbl = gOv9;
        *(u16 *)(rec + 0x64) = v;
        Actor_Apply101(10, tbl);
    }
    Actor_unk87_2(20);
    Actor_unk88_2((s32)&Value_0000030d);
    Actor_unk320();
}

void Scene_RunScene372(void)
{
    extern u8 gOv12[];

    u32 i;
    s32 record;

    Actor_unk321(20);
    Actor_unk21_2(0x166);
    Actor_unk322(0);
    Actor_unk323(1);
    Actor_unk324(2);
    Actor_unk325(3);
    Actor_unk326(4);
    Actor_unk327(5);
    Actor_unk20_4(0x10003, 1);
    Actor_unk21_4(0x10000, 2);
    Actor_unk328(1);
    Actor_unk329(120);
    Actor_unk330(0, 0);
    Actor_unk331(60);
    Actor_unk332(60);
    Actor_unk22_2(0x166);
    Actor_unk333(0);
    Actor_unk334(1);
    Actor_unk335(2);
    Actor_unk336(3);
    Actor_unk337(4);
    Actor_unk338(5);
}

void ActorDraw_SelectActorTwentySevenState(void)
{
    extern s32 gOv12;

    struct Actor *actor = Actor_unk52_4(27);
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

    rec8 = Actor_unk30(0);
    rec7 = Actor_unk31(8);
    Actor_unk339();
    if (Actor_unk32(0x305) != 0) {
        Actor_unk340(8);
        Actor_unk341(10);
        Actor_unk342(8, 2);
        Actor_unk343(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_unk344(8, 7);
        } else {
            Actor_unk345(8, 8);
        }
        Actor_unk346(8, 2);
        Actor_unk347(20);
        Actor_unk23_2(0xed2);
        Actor_unk348(8, 0);
        Actor_unk33(8, 0x200cec8);
        Actor_unk349(8, 6);
    } else {
        Actor_unk350(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Actor_unk96_3(8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_unk351(8, 7);
        } else {
            Actor_unk352(8, 8);
        }
        Actor_unk353(20);
        Actor_unk24_2(0xed1);
        Actor_unk354(8, 0, 20);
        Actor_unk355(8, 1);
        Actor_unk356(8, 4, 0);
        Actor_unk357(80);
        Actor_unk358(8, 2);
        Actor_unk359(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_unk360(8, 7);
        } else {
            Actor_unk361(8, 8);
        }
        Actor_unk362(2);
        Actor_unk363(8, 2, 0);
        Actor_unk364(60);
        Actor_unk365(8, 2);
        Actor_unk366(20);
        Actor_unk367(8, 0);
        Actor_unk34(8, 0x200cec8);
        Actor_unk368(8, 6);
        Actor_unk25_2(0x305);
    }
    Actor_unk369();
}

void Scene_ConfigureActorTwentyTwoScene(void)
{
    extern u8 gOv12[];

    u32 i;
    u8 *record;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Motion_EnableReset_1(ACTOR_ID);
    Actor_unk26_2(0x200c5b9);
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
    Actor_unk370(1);
    SceneWork_SetStepValue_1(0xed3);
    BattleEv_RunWait_1(0x1016, 0);
    Motion_SetHPosTerrain_3(ACTOR_ID, 0xac0000, 0x4fe0000);
    Actor_unk371(1);
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

    Actor_unk6_5(16, 96, 11, 73, 6, 3); /* main:08009180 */
    Actor_unk7_5(16, 96, 34, 68, 14, 10); /* main:08009180 */
    Actor_unk8_5(16, 96, 64, 68, 7, 7); /* main:08009180 */
    Actor_unk9_5(9, 95, 11, 73, 6, 3); /* main:08009180 */
    Actor_unk10_5(40, 94, 34, 68, 14, 10); /* main:08009180 */
    Actor_unk11_5(54, 94, 64, 68, 8, 7); /* main:08009180 */
    Actor_unk12_5(72, 75, 72, 76, 1, 1); /* main:08009180 */
    Actor_unk13_5(72, 75, 74, 76, 1, 1); /* main:08009180 */
    Actor_unk14_5(7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Actor_unk15_5(8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Actor_unk16_5(8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Actor_unk17_5(8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Actor_unk18_5(11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Actor_unk19_5(12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Actor_unk20_5(25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Actor_unk372(); /* main:08009128 */
}

void State_SetWords1c0And1c8AndRun(void)
{
    void Actor_unk66_4(s32);

    u8 *state;

    Actor_unk373();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Actor_unk89_2(0x87c);
    Actor_Apply102(12, 2);
    Actor_unk90_2(0x900);
    Actor_unk374();
}

void State_SetWorkWordsAndFlag87f(void)
{
    void Actor_unk66_4(s32);

    u8 *state;

    Actor_unk375();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Actor_unk91_2(0x87f);
    Actor_Apply103(12, 3);
    Actor_unk92_2(0x900);
    Actor_unk376();
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
            Actor_unk93_2(o);
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
        Actor_unk94_2(obj);
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
        Actor_unk95_2(o);
    } else {
        d = Actor_unk93(t << 10);
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
        Actor_unk96_2(o);
    } else {
        d = Actor_unk94(t << 10);
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
    void Actor_unk66_4(s32);

    Actor_Apply112(140, 0);
}

void Scene_RunSingleStep(void)
{
    Actor_unk377();
}

void Scene_RunFourPairedSteps(void)
{
    extern u8 gOv12[];

    Actor_unk97_2(32);
    Actor_unk378();
    Actor_unk98_2(33);
    Actor_unk379();
    Actor_unk99_2(30);
    Actor_unk380();
    if (*(s32 *)0x0200d7f8 == 0) {
        Actor_unk100_2(29);
        Actor_unk381();
    }
}

void State_SetValue19ThenCall(void)
{
    void Actor_unk66_4(s32);

    Actor_unk101_2(19);
    Actor_unk382();
}

void OvObj_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = Actor_unk22_4(0)->f50;
    dst = Actor_unk23_4(22)->f50;
    dst->f = src->f;
    dst2 = Actor_unk24_4(8)->f50;
    dst2->f = src->f;
}

void State_SetValueEe4(void)
{
    void Actor_unk66_4(s32);

    Actor_unk383();
    Actor_Apply113(0xee4, 1);
    Actor_unk384();
}

void Scene_RunScriptedStep1120(void)
{
    extern u8 gOv12[];

    Actor_unk385();
    Actor_Apply114((s32)&Value_00001120, 1);
    Actor_unk386();
}
