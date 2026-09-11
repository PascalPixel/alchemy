#include "types.h"
#include "scene.h"
#include "resource_38f.h"
#include "object_runtime.h"

/* overlays/scene/field/extended_sequence/extended_actor_sequence_scene.c */
#define SCENE_POINTER (*(u8 *volatile *)0x03001ebc)

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

struct Ent {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    u8 pad0c[4];
    s32 f10;
    u8 pad14[0x46];
    u8 f5a;
    u8 pad5b[13];
    struct Ent *f68;
};

struct Rec { u16 f00, f02, f04, f06; };

struct Ent {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14;
    s32 f18;
    s32 f1c, f20, f24, f28, f2c, f30, f34;
    s32 f38, f3c, f40;
};

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

struct Obj2 {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

extern s16 gCell[];
extern u8 Value_00000027;
extern u8 Value_00000026;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 Value_00000024;
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gWork[];
extern u8 gOv10[];
extern u8 gOv11[];
extern s32 gIw;
extern s32 gOv12;
extern u8 gOv13[];

struct Rec *Field_unk7_4(s32);

struct Ent *Field_unk8_4(s32);
struct Ent *Field_unk9_4(s32);
struct Ent *Field_unk10_4(s32);
struct Ent *Field_unk11_4(s32);

struct Obj *Field_unk12_4(s32, s32, s32, s32);

/* Each Func_ symbol above names the call word the image holds before loader
 * relocation, not a runtime address. */

/* Calls spelled through this wrapper pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is shared with later uses in the same
 * block. A value-returning call also sets r0 last of its arguments. */

s32 State_FlushPendingWordB698(s32 arg0)
{
    if (*(s32 *)0x0200B698 != 0) {
        Field_Apply(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}

s32 Effect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Field_unk7(p);
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Field_Apply2(p, 7);
    } else {
        Field_Apply3(p, 10);
    }
    if ((s16)p->f64 > 1200) {
        p->f64 = 0;
    }
    return 1;
}

void SceneData_InitRecordTable(u8 *o)
{
    u8 *p = o + 72;
    u32 i;
    s32 normal;
    s32 special;

    i = 0;
    normal = 105;
    special = 110;
    for (; i <= 8; i++) {
        *(u16 *)p = normal;
        if ((u32)(i - 6) <= 1) {
            *(u16 *)p = special;
        }
        p[22] = 2;
        *(s32 *)(p + 4) = 1;
        p += 24;
    }
}

s32 Effect_UpdateFallingObject(struct Obj *p)
{
    p->f08 += p->f24;
    p->f10 += p->f2c;
    p->f2c -= 2621;
    p->f18 += 0x600;
    p->f1c += 0x600;
    {
        s32 t = p->f64 - 1;
        p->f64 = t;
        if ((u16)t == 0) {
            Field_unk3_2(p);
        }
    }
    return 1;
}

s32 Actor_TurnTowardTarget(struct Ent *p)
{
    struct Ent *q;
    u16 h;
    s32 t;
    s32 v;
    u8 *b;

    q = p->f68;
    if (q != 0) {
        b = &p->f5a;
        v = 0xfe;
        v &= *b;
        *b = v;
        h = Field_Apply4(q->f10 - p->f10, q->f08 - p->f08);
        t = h;
        t -= p->f06;
        t <<= 16;
        t >>= 16;
        if (t != 0) {
            if (t > 0x1000) {
                t = 0x1000;
            }
            if (t < -0x1000) {
                t = -0x1000;
            }
            p->f06 = p->f06 + t;
        }
    }
    return 1;
}

s32 SceneData_SelectTableAe60BySelector(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000027) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_00000026) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 SceneData_GetTableB010ForSelector26(void)
{
    if (gCell[224] == (s32)&Value_00000026) {
        return (s32)gOv4;
    }
    return 0;
}

/*
 * Table getter for resource_38f. The owner at 0x020001d4 is eight bytes and
 * includes its one pool word at 0x020001d8: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTableB040(void)
{
    return (u8 *)0x0200b040;
}

s32 SceneData_SelectTableB080BySelector(void)
{
    s32 v = gCell[224];
    if (v == (s32)&Value_00000024) {
        if (Field_unk8(0x845) == 0) {
            Field_unk4_2((s32)gOv5);
        }
        return (s32)gOv5;
    }
    if (v == (s32)&Value_00000027) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

void Scene_RunActor16MessageBranch(void)
{
    struct Rec *q = Field_unk7_4(0);
    s32 v = q->f06;
    Field_unk5_2(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Field_unk6_2(16);
    } else {
        Field_unk7_2(0x16b3);
        Field_Apply5(16, 0);
    }
    Field_unk13_4();
}

void Scene_RunActor27Step(void)
{
    Field_unk9_3(27, 0, 1);
}

s32 SceneData_SelectTableB3b0BySelector(void)
{
    if (gCell[224] == (s32)&Value_00000027) {
        return (s32)gOv8;
    }
    return (s32)gOv9;
}

void Scene_ConfigureActor0ThenRun(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Field_unk9(0) + 85) = 0;
    Field_Place(0, 0x8000, 0x4000);
    Field_unk14_4(0, 2);
    Field_unk2_3(0, 0, -8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Field_unk15_4(a0);
}

s32 Scene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;

    step = gCell[224];

    if (step == (s32)&Value_00000027) {
        Field_unk16_4();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Field_unk17_4(Field_unk10(23), 0);
    Field_unk18_4(Field_unk11(24), 0);
    Field_unk19_4(Field_unk12(25), 0);
    Field_unk20_4(Field_unk13(26), 0);

    tbl = gOv10;
    Field_unk21_4(23, tbl);
    Field_unk22_4(24, tbl);
    Field_unk23_4(25, tbl);
    Field_unk24_4(26, tbl);

    if (Field_unk14(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Field_unk25_4(Field_unk15(actor), 0);
        }
        Field_unk26_4(13, 9, 1, 1, 13, 8);
        Field_unk27_4(13, 9, 1, 1, 15, 8);
        {
            s32 m = 14;
            s32 n = 9;
            Field_unk28_4(13, 9, 1, 1, m, n);
        }
    }

    if (Field_unk16(0x843) == 0) {
        if (gCell[225] == 1) {
            Field_unk29_4();
        }
    }

    if (Field_unk17(0x843) != 0) {
        Field_unk30_4(1);
        Field_unk31_4(2);
        Field_unk32_4(3);
        Field_unk33_4(17);
        Field_unk34_4(18);
        Field_unk35_4(19);
        Field_unk36_4(20);
        Field_unk37_4(21);
        Field_unk38_4(22);
        Field_unk39_4(gOv11);
    }

    return 0;
}

void Actor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent *p;

    p = Field_unk8_4(19);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = *(u32 *)0x03001e40 & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Field_unk9_4(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Field_unk10_4(21);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = *(u32 *)0x03001e40 & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Field_unk11_4(22);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
}

void Scene_StartEffect141Sequence(s32 arg0, s32 arg1)
{
    Field_Apply6(141, 1);
    Field_Apply7(arg0, arg1);
    Field_unk40_4();
    Field_unk8_2(1);
    Field_unk9_2(1);
}

void Scene_RunSequenceA(void)
{
    Field_unk10_2(2);
    Field_unk41_4();
    Field_unk42_4();
}

void Scene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Field_Apply8(arg0, 0);
    Field_unk11_2(arg1);
}

void Scene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Field_unk10_3(arg0, arg1, 0);
    Field_unk12_2(arg2);
}

s32 Effect_AdvanceAngleUntilIdle(struct Obj *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Field_unk13_2(p);
    }
    return 1;
}

void Effect_SpawnObject26EveryEightFrames(void)
{
    struct Obj2 *p;
    struct Sub *q;
    s32 f;
    s32 v;
    s32 w;
    s32 c1 = 0x620000;
    s32 c2 = 0x690000;
    s32 c3 = 0x620000;
    s32 c4 = 0x010d0000;

    f = gIw & 7;
    if (f != 0) {
        return;
    }
    if (gOv12 != 0) {
        Field_unk14_2(200);
    }
    p = Field_unk12_4(26, c1, 0, c2);
    if (p == 0) {
        return;
    }
    q = p->f50;
    q->f26 = f;
    v = 0xfe;
    v &= p->f23;
    p->f23 = v;
    w = ~12;
    w &= q->f09;
    w |= 4;
    q->f09 = w;
    p->f18 = 0x1999;
    p->f30 = 0x80000;
    p->f34 = 0x80000;
    p->f55 = f;
    Field_Apply9(p, 2);
    Field_SetMode(p, c3, 0, c4);
    Field_Apply10(p, gOv13);
}

s32 Effect_SetModeByFrameBit1(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Field_Apply11(arg0, 10);
    } else {
        Field_Apply12(arg0, 7);
    }
    return 0;
}

void Scene_RunFlag845And847Branches(void)
{
    if (Field_unk18(0x845) == 0) {
        Field_unk11_3(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Field_SetRect(9, 17, 5, 1, k5, k6); }
        Field_unk43_4();
        Field_unk15_2(1);
    } else {
        Field_unk12_3(9, 0, 0);
    }
    if (Field_unk19(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Field_unk2_6(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Field_unk3_6(23, 11, 5, 7, k5, k6); }
        Field_unk44_4();
        Field_unk16_2(1);
    }
}

/* overlays/scene/field/extended_sequence/run_extended_actor_sequence.c */

struct ObjectRuntime *Field_unk45_4();

/* The inline adapters retain argument lifetimes at constant-valued calls.
 * Their return types follow the target helpers. */

static __inline__ struct ObjectRuntime *Pointer1(struct ObjectRuntime *(*f)(), s32 a0)
{
    return f(a0);
}

extern s32 gOv14;

extern s32 gOv15;
extern s32 gOv16;
extern s32 gOv17;
extern s32 gOv18;
extern s32 gOv12;
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];
extern u8 gOv22[];
extern u8 gOv23[];
extern u8 gOv24[];
extern u8 gOv25[];
extern u8 gOv26[];
extern u8 gOv27[];
extern u8 gOv28[];

void Scene_RunExtendedActorSequence(void)
{
    struct ObjectRuntime *record;
    s32 flag_addr;
    s32 mask;
    s32 value;
    s32 action_a;
    s32 action_b;
    s32 step_addr;
    s32 work_addr;
    s32 action_c;
    s32 action_d;
    s32 step_next;

    Field_unk46_4();
    flag_addr = (s32)&gOv14;
    *(s32 *)flag_addr = Field_Check(3);
    record = Field_unk45_4(19);
    Field_unk47_4((s32)record, 0);
    record = Field_unk45_4(20);
    Field_unk47_4((s32)record, 0);
    record = Field_unk45_4(21);
    Field_unk47_4((s32)record, 0);
    record = Field_unk45_4(22);
    Field_unk47_4((s32)record, 0);
    Field_Run(0x680000, -1, 0x1000000, 0);
    Field_unk48_4();
    Field_unk3_3(0, 0x170000, 0xf70000);
    Field_unk49_4(1);
    Field_unk50_4();
    Field_unk51_4();
    Field_unk4_3(0, 0xcccc, 0x6666);
    Field_unk52_4(0, 121, 238);
    Field_unk4_3(1, 0x9999, 0x4ccc);
    Field_unk4_3(2, 0x9999, 0x4ccc);
    record = Pointer1(Field_unk45_4, 0);
    if ((s32)record != 0) {
        Field_unk53_4(1, record->x, record->z);
    }
    record = Pointer1(Field_unk45_4, 0);
    if ((s32)record != 0) {
        Field_unk53_4(2, record->x, record->z);
    }
    Field_unk2_4(1, (s32)gOv22);
    Field_unk54_4(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_unk4_3(3, 0x9999, 0x4ccc);
        record = Pointer1(Field_unk45_4, 0);
        if ((s32)record != 0) {
            Field_unk53_4(3, record->x, record->z);
        }
        Field_unk54_4(3, (s32)gOv24);
    }
    Field_unk55_4(2);
    Field_unk5_3(0, 0xe000, 20);
    Field_unk5_3(2, 0xe000, 20);
    Field_unk5_3(1, 0x2000, 20);
    flag_addr = (s32)&gOv14;
    if (*(s32 *)flag_addr != 0) {
        Field_unk5_3(3, 0x2000, 0);
    }
    Field_unk5_3(0, 0x2000, 20);
    Field_unk5_3(2, 0x2000, 20);
    Field_unk5_3(1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_unk5_3(3, 0xe000, 0);
    }
    Field_unk5_3(0, 0x8000, 0);
    Field_unk56_4(2, 0, 0);
    Field_unk56_4(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_unk56_4(3, 0, 0);
    }
    Field_unk57_4(40);
    Field_unk58_4(1, 1);
    Field_Do(0x1473);
    Scene_RunPairedStepA(1, 20);
    Field_unk56_4(0, 1, 0);
    Field_unk59_4(0, 3);
    Field_unk57_4(30);
    Field_unk6_3(2, 72, 0x11e);
    Field_unk6_3(2, 72, 0x12e);
    Field_unk6_3(2, 88, 0x136);
    Field_unk60_4(2, 1);
    Field_unk56_4(2, 0, 0);
    Field_unk57_4(20);
    Field_unk56_4(0, 2, 0);
    Field_unk56_4(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_unk56_4(3, 2, 0);
    }
    Field_unk57_4(30);
    Field_unk61_4(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Field_unk61_4(3, 3);
    }
    Field_unk59_4(0, 3);
    Field_unk57_4(20);
    Field_unk59_4(2, 3);
    Field_unk57_4(30);
    Field_unk62_4(2, 0, 0);
    Field_unk57_4(20);
    Scene_StartEffect141Sequence(2, 9);
    Field_unk57_4(40);
    Scene_RunSequenceA();
    Field_unk7_3(2, 0x100, 40);
    Field_unk4_3(2, 0x10000, 0x8000);
    Field_unk45_4(2)->action_flags &= 254;
    Field_unk6_3(2, 80, 0x136);
    mask = 1;
    Field_unk57_4(1);
    Field_unk45_4(2)->action_flags |= mask;
    Field_unk7_3(1, 0x102, 40);
    Scene_RunPairedStepA(1, 20);
    Field_unk7_3(2, 0x102, 40);
    Scene_RunPairedStepA(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Field_unk56_4(3, 0, 0);
    }
    Field_unk63_4(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_unk5_3(3, 0xc000, 0);
    }
    Field_unk5_3(1, 0xc000, 0);
    Field_unk5_3(0, 0xe000, 0);
    Field_unk58_4(1, 1);
    Scene_RunPairedStepA(1, 20);
    Field_unk4_3(2, 0x8000, 0x4000);
    Field_unk45_4(2)->action_flags &= 254;
    Field_unk6_3(2, 72, 0x11e);
    Field_unk57_4(1);
    Field_unk45_4(2)->action_flags |= mask;
    Field_unk2_4(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_unk7_3(3, 0x105, 0);
        Field_unk57_4(60);
        Scene_RunPairedStepA(3, 20);
        Field_unk61_4(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk56_4(2, 0, 0);
    Field_unk61_4(0, 3);
    Field_unk61_4(1, 3);
    Field_unk59_4(2, 3);
    Field_unk57_4(20);
    Field_unk3_4(1, 0x102);
    Field_unk57_4(10);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_unk2(1, 0);
    if (Field_unk3(0, 0) == 0) {
        Field_unk57_4(20);
        Field_unk61_4(1, 3);
    } else {
        Field_unk59_4(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Scene_RunPairedStepA(1, 40);
    Scene_RunPairedStepB(2, 0x2000, 40);
    Scene_RunPairedStepB(2, 0x8000, 20);
    Scene_RunPairedStepB(2, 0x4000, 40);
    Field_unk7_3(2, 0x101, 0);
    Field_unk57_4(60);
    Field_unk62_4(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 60);
    value = 160;
    Scene_RunPairedStepB(3, 0x2000, 10);
    Field_unk62_4(1, 0x2000, 0);
    Scene_RunPairedStepB(0, (value << 8), 10);
    Field_unk7_3(1, 0x101, 0);
    Field_unk7_3(0, 0x101, 0);
    Field_unk57_4(40);
    Field_unk62_4(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Field_unk60_4(1, 2);
    Field_Do(0x147b);
    Scene_RunPairedStepA(1, 10);
    Scene_RunPairedStepB(2, 0xc000, 20);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(2, 20);
    Scene_RunPairedStepB(1, 0, 20);
    Scene_RunPairedStepB(0, (value << 8), 40);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Scene_RunPairedStepB(0, 0x6000, 30);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(0, 0xe000, 30);
    Field_unk60_4(2, 2);
    Field_unk7_3(2, 0x100, 0);
    Field_unk57_4(40);
    Field_unk62_4(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk64_4(17);
    Field_unk64_4(206);
    Field_unk4_4(0x7fff, 0);
    Field_unk65_4(1);
    Field_unk49_4(1);
    gOv12 = 1;
    Field_unk4((s32)gOv20, 0xc80);
    Field_unk49_4(20);
    Field_unk4_4(0x405210, 1);
    Field_unk4_4(0x10000, 2);
    Field_unk65_4(120);
    Field_unk49_4(60);
    action_a = (s32)gOv25;
    Field_unk54_4(0, action_a);
    Field_unk54_4(1, action_a);
    Field_unk54_4(2, action_a);
    Field_unk54_4(3, action_a);
    Field_unk57_4(100);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Field_unk57_4(40);
        Field_unk3_4(3, 0x102);
        Field_unk57_4(40);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk57_4(20);
    if (gOv14 != 0) {
        value = 128;
        record = Field_unk45_4(3);
        record->velocity_y = (value << 10);
        Field_unk57_4(10);
        Field_unk66_4(3, (value << 10), (value << 10));
        Field_unk8_3(3, -2, 0);
        Field_unk2_4(3, (s32)gOv26);
        record = Field_unk45_4(3);
        Field_unk47_4((s32)record, 0);
        Field_unk61_4(3, 19);
        Field_unk57_4(10);
    }
    value = 128;
    record = Field_unk45_4(0);
    record->velocity_y = (value << 10);
    Field_unk57_4(10);
    Field_unk4_3(0, (value << 10), (value << 10));
    action_b = (s32)gOv26;
    Field_unk54_4(0, action_b);
    record = Field_unk45_4(0);
    Field_unk47_4((s32)record, 0);
    Field_unk61_4(0, 19);
    Field_unk57_4(20);
    record = Pointer1(Field_unk45_4, 1);
    record->velocity_y = (value << 10);
    Field_unk57_4(10);
    Field_unk4_3(1, (value << 10), (value << 10));
    Field_unk54_4(1, action_b);
    record = Field_unk45_4(1);
    Field_unk47_4((s32)record, 0);
    Field_unk61_4(1, 19);
    Field_unk57_4(40);
    record = Pointer1(Field_unk45_4, 2);
    record->velocity_y = (value << 10);
    Field_unk57_4(10);
    Field_unk54_4(2, action_b);
    record = Field_unk45_4(2);
    Field_unk47_4((s32)record, 0);
    Field_unk61_4(2, 19);
    gOv12 = 0;
    Field_unk57_4(160);
    Field_unk5((s32)gOv20);
    Field_unk57_4(120);
    Field_unk4_4(0x406218, 1);
    Field_unk65_4(60);
    Field_unk49_4(60);
    gOv16 = 0;
    step_addr = (s32)&gOv17;
    gOv15 = 0x800000;
    *(s32 *)step_addr = 1;
    Field_unk4((s32)gOv21, 0xc80);
    Field_unk57_4(180);
    Field_unk64_4(21);
    Scene_RunPairedStepA(1, 80);
    Scene_RunPairedStepA(2, 40);
    Field_unk3_4(0, 0x102);
    Field_unk3_4(1, 0x102);
    Field_unk3_4(2, 0x102);
    Field_unk3_4(3, 0x102);
    Field_unk57_4(60);
    Scene_RunPairedStepA(2, 20);
    *(s32 *)step_addr = 2;
    Field_unk58_4(2, 2);
    Field_unk57_4(20);
    Field_unk58_4(1, 1);
    Field_unk57_4(40);
    Field_unk58_4(0, 2);
    Field_unk58_4(3, 1);
    Field_unk57_4(20);
    Field_unk58_4(2, 3);
    Field_unk57_4(40);
    Field_unk58_4(0, 1);
    Field_unk57_4(20);
    Field_unk5_4(1, 2);
    Field_unk57_4(20);
    Field_unk58_4(3, 2);
    Field_unk3_4(1, 0x102);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_unk3_4(3, 0x102);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    work_addr = (s32)&gOv17;
    *(s32 *)work_addr = 3;
    Field_unk45_4(0)->unknown_23 &= 254;
    Field_unk45_4(1)->unknown_23 &= 254;
    Field_unk45_4(2)->unknown_23 &= 254;
    Field_unk45_4(3)->unknown_23 &= 254;
    Field_unk67_4(0, 3);
    Field_unk67_4(1, 3);
    Field_unk67_4(2, 3);
    value = 0;
    Field_unk67_4(3, 3);
    gOv18 = value;
    Field_unk4((s32)gOv19, 0xc80);
    Field_unk64_4(220);
    Field_unk45_4(19)->unknown_23 &= 254;
    Field_unk67_4(19, 2);
    Field_unk3_3(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_unk54_4(19, action_c);
    Field_unk45_4(20)->unknown_23 &= 254;
    Field_unk67_4(20, 2);
    Field_unk3_3(20, 0x640000, 0x1120000);
    Field_unk54_4(20, action_c);
    if (gOv14 != 0) {
        Field_unk45_4(21)->unknown_23 &= 254;
        Field_unk67_4(21, 2);
        Field_unk3_3(21, 0x4a0000, 0xfe0000);
        Field_unk54_4(21, action_c);
    }
    Field_unk45_4(22)->unknown_23 &= 254;
    Field_unk67_4(22, 2);
    Field_unk3_3(22, 0x5e0000, 0xe10000);
    Field_unk54_4(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Field_unk49_4(1);
        } while (gOv17 != 0);
    }
    Field_unk2_2(0x12c);
    Field_unk5((s32)gOv21);
    Field_unk57_4(120);
    Field_unk64_4(17);
    Field_unk4_4(0x10000, 1);
    Field_unk65_4(60);
    Field_unk49_4(60);
    Field_unk68_4(19);
    Field_unk68_4(20);
    if (gOv14 != 0) {
        Field_unk68_4(21);
    }
    (Field_unk68_4)(22);
    Field_unk49_4(1);
    action_d = (s32)gOv28;
    (Field_unk54_4)(19, action_d);
    Field_unk54_4(20, action_d);
    if (gOv14 != 0) {
        Field_unk54_4(21, action_d);
    }
    Field_unk69_4(22, action_d);
    Field_unk57_4(80);
    Field_unk60_4(1, 2);
    Field_unk57_4(40);
    Field_unk2(1, 0);
    Field_unk3_3(17, 0x570000, 0x8b0000);
    Field_unk3_3(18, 0x570000, 0x8b0000);
    Field_unk49_4(1);
    if (Field_unk3(17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk60_4(0, 1);
    Field_unk57_4(20);
    Field_unk6_4(2, 2);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Field_unk60_4(3, 2);
        Field_unk57_4(10);
        Field_Do(0x1488);
        Scene_RunPairedStepA(3, 40);
    }
    Field_unk58_4(1, 1);
    Field_unk7_3(1, 0x101, 0);
    Field_unk57_4(80);
    Field_unk60_4(2, 2);
    Field_Do(0x1489);
    Scene_RunPairedStepA(2, 40);
    Field_unk60_4(1, 3);
    Field_unk57_4(40);
    Field_unk67_4(1, 2);
    Field_unk45_4(1)->unknown_23 |= 1;
    record = Field_unk45_4(1);
    Field_unk47_4((s32)record, 1);
    Field_unk70_4(1, 6, 0);
    Field_unk61_4(1, 1);
    Scene_RunPairedStepB(1, 0x4000, 60);
    Scene_RunPairedStepA(1, 20);
    Field_unk58_4(1, 2);
    (Scene_RunPairedStepA)(1, 10);
    Field_unk60_4(0, 3);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Field_unk7_3(1, 0x101, 0);
    Field_unk57_4(40);
    Scene_RunPairedStepB(1, 0x6000, 40);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_unk70_4(1, 2, 0);
    Field_unk57_4(40);
    Field_unk70_4(1, 2, 0);
    Field_unk57_4(10);
    Field_unk70_4(1, 4, 0);
    Field_unk57_4(20);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_unk7_3(3, 0x100, 0);
        Field_unk57_4(60);
        Field_unk60_4(3, 2);
        Field_unk57_4(80);
        Field_unk67_4(3, 2);
        Field_unk45_4(3)->unknown_23 |= 1;
        record = Field_unk45_4(3);
        Field_unk47_4((s32)record, 1);
        Field_unk70_4(3, 4, 0);
        Field_unk8_3(3, -2, 0);
        Field_unk61_4(3, 1);
        Scene_RunPairedStepB(3, 0xe000, 60);
        Field_unk60_4(3, 2);
        Field_unk57_4(20);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk70_4(1, 2, 0);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Field_unk59_4(1, 3);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepA(1, 20);
    Field_unk59_4(1, 3);
    Field_unk57_4(10);
    Field_unk60_4(2, 1);
    Field_unk57_4(40);
    Field_unk60_4(2, 2);
    Field_unk57_4(20);
    value = 1;
    Field_unk67_4(2, 2);
    Field_unk45_4(2)->unknown_23 |= value;
    record = Field_unk45_4(2);
    Field_unk47_4((s32)record, 1);
    Field_unk70_4(2, 4, 0);
    Field_unk61_4(2, 1);
    Field_unk5_3(2, 0xc000, 0);
    Field_unk60_4(0, 2);
    Field_unk57_4(10);
    Field_unk67_4(0, 2);
    Field_unk45_4(0)->unknown_23 |= value;
    record = Field_unk45_4(0);
    Field_unk47_4((s32)record, 1);
    Field_unk70_4(0, 4, 0);
    Field_unk61_4(0, 1);
    Scene_RunPairedStepB(0, 0x6000, 60);
    Field_unk7_3(0, 0x105, 0);
    Field_unk7_3(2, 0x105, 0);
    Field_unk57_4(60);
    Scene_RunPairedStepB(0, 0xa000, 20);
    Field_unk59_4(1, 3);
    Field_unk59_4(0, 3);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_unk5_3(2, 0xc000, 0);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(2, 20);
    Field_unk61_4(0, 3);
    Field_unk61_4(3, 3);
    Field_unk59_4(1, 3);
    Field_unk57_4(20);
    Field_unk60_4(2, 1);
    Field_unk57_4(20);
    Field_unk2(2, 0);
    Field_unk61_4(2, 3);
    Field_unk5_3(2, 0xe000, 0);
    Field_unk62_4(1, 0x2000, 0);
    if (Field_unk3(0, 0) == 0) {
        Field_unk61_4(2, 3);
        Field_unk59_4(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Field_unk60_4(1, 2);
        Scene_RunPairedStepB(1, 0x2000, 10);
        Field_unk58_4(1, 2);
        Field_unk71_4(1, 0);
    }
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_unk59_4(1, 4);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(2, 10);
    if (gOv14 != 0) {
        Field_unk60_4(3, 2);
        Scene_RunPairedStepB(3, 0, 20);
        Scene_RunPairedStepB(3, 0x2000, 10);
        Field_unk61_4(3, 4);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk5_3(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_unk61_4(0, 3);
    Field_unk59_4(1, 3);
    value = 128;
    Field_unk57_4(20);
    Field_unk5_3(0, 0x6000, 0);
    Scene_RunPairedStepB(1, (value << 7), 10);
    Field_unk59_4(2, 4);
    Field_unk57_4(20);
    Field_unk7_3(0, 0x102, 0);
    Field_unk7_3(1, 0x102, 80);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_unk58_4(2, 2);
    Scene_RunPairedStepA(2, 20);
    Field_unk5_3(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 40);
    Field_unk62_4(1, (value << 7), 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_unk3_4(1, 0x102);
    Field_unk57_4(40);
    Scene_RunPairedStepA(1, 20);
    Field_unk59_4(2, 3);
    Field_unk57_4(20);
    Field_unk3_4(1, 0x102);
    Field_unk57_4(40);
    Scene_RunPairedStepA(1, 20);
    Field_unk61_4(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_unk60_4(1, 2);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_unk2(1, 0);
    Field_unk62_4(0, 0xa000, 0);
    if (Field_unk3(0, 0) == 0) {
        Field_unk59_4(1, 3);
    } else {
        Field_unk57_4(20);
        Field_unk60_4(1, 2);
        Field_unk57_4(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk71_4(1, 0);
    Field_unk64_4(21);
    Field_unk4_4(0x406218, 1);
    Field_unk65_4(60);
    Field_unk49_4(60);
    gOv16 = 0;
    gOv15 = 0x800000;
    step_next = (s32)&gOv17;
    *(s32 *)step_next = 1;
    Field_unk4((s32)gOv21, 0xc80);
    Field_unk57_4(80);
    Field_unk58_4(0, 2);
    Field_unk5_4(1, 2);
    Field_unk58_4(3, 2);
    Field_unk60_4(2, 2);
    Field_unk57_4(60);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_Do(0x149d);
    Scene_RunPairedStepA(2, 10);
    Scene_RunPairedStepB(1, 0xc000, 10);
    Scene_RunPairedStepB(0, 0xc000, 10);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0xc000, 10);
    }
    Field_unk45_4(0)->unknown_23 &= 254;
    Field_unk45_4(1)->unknown_23 &= 254;
    Field_unk45_4(2)->unknown_23 &= 254;
    Field_unk45_4(3)->unknown_23 &= 254;
    Field_unk67_4(0, 3);
    Field_unk67_4(1, 3);
    Field_unk67_4(2, 3);
    Field_unk67_4(3, 3);
    *(s32 *)step_next = 2;
    Field_unk64_4(220);
    Field_unk3_3(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_unk54_4(19, action_c);
    Field_unk3_3(20, 0x640000, 0x1120000);
    Field_unk54_4(20, action_c);
    if (gOv14 != 0) {
        Field_unk3_3(21, 0x4a0000, 0xfe0000);
        Field_unk54_4(21, action_c);
    }
    Field_unk3_3(22, 0x5e0000, 0xe10000);
    Field_unk2_4(22, action_c);
    Field_unk57_4(120);
    *(s32 *)step_next = 3;
    do {
        Field_unk49_4(1);
    } while (gOv17 != 0);
    Scene_RunPairedStepA(17, 80);
    Scene_RunPairedStepA(18, 20);
    Field_unk7_3(0, 0x101, 0);
    Field_unk7_3(1, 0x101, 0);
    Field_unk7_3(2, 0x101, 0);
    Field_unk7_3(3, 0x101, 0);
    Field_unk57_4(60);
    Scene_RunPairedStepA(18, 20);
    Field_unk61_4(0, 3);
    Field_unk61_4(1, 3);
    Field_unk61_4(3, 3);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_unk61_4(0, 3);
    Field_unk61_4(1, 3);
    Field_unk61_4(3, 3);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_unk7_3(0, 0x100, 0);
    Field_unk7_3(1, 0x100, 0);
    Field_unk7_3(2, 0x100, 0);
    Field_unk7_3(3, 0x100, 0);
    Field_unk57_4(40);
    Scene_RunPairedStepA(17, 10);
    Field_unk5_3(0, 0x8000, 0);
    Field_unk5_3(1, 0x4000, 0);
    Field_unk62_4(3, 0, 0);
    Scene_RunPairedStepB(2, 0xc000, 40);
    Scene_RunPairedStepA(18, 10);
    Field_unk3_4(0, 0x102);
    Field_unk3_4(1, 0x102);
    Field_unk3_4(2, 0x102);
    Field_unk3_4(3, 0x102);
    Field_unk62_4(0, 0xc000, 0);
    Field_unk62_4(1, 0xc000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 80);
    Scene_RunPairedStepA(18, 10);
    Field_unk5_3(0, 0x8000, 0);
    Field_unk5_3(1, 0x4000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 40);
    Scene_RunPairedStepA(17, 10);
    Field_unk62_4(0, 0xc000, 0);
    Field_unk62_4(1, 0xc000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Field_unk61_4(0, 4);
    Field_unk61_4(1, 4);
    Field_unk61_4(3, 4);
    Field_unk59_4(2, 4);
    Field_unk57_4(60);
    Scene_RunPairedStepA(18, 10);
    Field_unk61_4(0, 3);
    Field_unk61_4(1, 3);
    Field_unk61_4(3, 3);
    Field_unk59_4(2, 3);
    Scene_RunPairedStepA(18, 20);
    Field_unk5_3(0, 0x8000, 0);
    Field_unk5_3(1, 0x4000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(18, 10);
    Field_unk58_4(0, 2);
    Field_unk58_4(1, 2);
    Field_unk58_4(3, 2);
    Field_unk60_4(2, 2);
    Field_unk62_4(0, 0xc000, 0);
    Field_unk62_4(1, 0xc000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Field_unk62_4(3, 0xc000, 0);
    Scene_RunPairedStepA(18, 20);
    Field_unk5_3(0, 0x8000, 0);
    Field_unk5_3(1, 0x4000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(17, 20);
    Field_unk3_4(0, 0x102);
    Field_unk3_4(1, 0x102);
    Field_unk3_4(3, 0x102);
    Field_unk3_4(2, 0x102);
    Field_unk57_4(40);
    Scene_RunPairedStepA(18, 10);
    Field_unk62_4(0, 0xc000, 0);
    Field_unk62_4(1, 0xc000, 0);
    Field_unk62_4(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Scene_RunPairedStepA(18, 10);
    Field_unk61_4(0, 3);
    Field_unk61_4(1, 3);
    Field_unk61_4(3, 3);
    Field_unk59_4(2, 3);
    Field_unk57_4(60);
    Field_unk71_4(18, 0);
    Field_unk71_4(17, 0);
    Field_unk5((s32)gOv21);
    Field_unk57_4(80);
    Field_unk4_4(0x10000, 1);
    Field_unk65_4(60);
    Field_unk49_4(80);
    Field_unk68_4(19);
    Field_unk68_4(20);
    work_addr = (s32)&gOv14;
    Field_unk68_4(21);
    Field_unk68_4(22);
    Field_unk49_4(1);
    action_d = (s32)gOv28;
    Field_unk54_4(19, action_d);
    Field_unk54_4(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Field_unk54_4(21, action_d);
    }
    Field_unk69_4(22, action_d);
    Field_unk57_4(20);
    Field_unk67_4(0, 2);
    Field_unk67_4(1, 2);
    Field_unk67_4(2, 2);
    value = 1;
    Field_unk67_4(3, 2);
    Field_unk45_4(0)->unknown_23 |= value;
    Field_unk45_4(1)->unknown_23 |= value;
    Field_unk45_4(2)->unknown_23 |= value;
    Field_unk45_4(3)->unknown_23 |= value;
    Field_unk60_4(2, 2);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_unk2(2, 0);
    Field_unk5_3(1, 0x2000, 0);
    Field_unk62_4(3, 0, 0);
    if (Field_unk3(0, 0) == 0) {
        Field_unk60_4(1, 2);
        Field_unk57_4(10);
        Field_unk2(1, 0);
        if (Field_unk3(0, 0) == 0) {
            Scene_RunPairedStepB(3, 0, 20);
            Field_unk7_3(1, 0x101, 0);
            Field_unk7_3(2, 0x101, 0);
            Field_unk7_3(3, 0x101, 0);
            Field_unk57_4(40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Scene_RunPairedStepA(1, 10);
            Scene_RunPairedStepB(2, 0xc000, 20);
            Scene_RunPairedStepB(2, 0xe000, 20);
            Field_unk59_4(2, 3);
            Scene_RunPairedStepA(2, 20);
            Scene_RunPairedStepB(1, 0x2000, 20);
        } else {
            Scene_RunPairedStepB(3, 0, 20);
            Field_unk7_3(1, 0x102, 0);
            Field_unk7_3(2, 0x102, 0);
            Field_unk7_3(3, 0x102, 40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Field_Do(0x14b4);
            Scene_RunPairedStepA(1, 20);
            Field_unk59_4(2, 3);
            Scene_RunPairedStepA(2, 20);
        }
        Field_unk61_4(3, 3);
        Field_unk59_4(1, 3);
    } else {
        Field_unk57_4(20);
        Field_unk59_4(1, 3);
        Field_unk57_4(10);
        Field_Do(0x14b6);
        Scene_RunPairedStepA(1, 10);
        Field_unk5_3(1, 0x4000, 0);
        Scene_RunPairedStepB(0, 0x6000, 20);
        Field_unk61_4(1, 3);
        Field_unk59_4(0, 3);
        Field_unk57_4(10);
        Field_unk59_4(2, 4);
        Field_unk2(2, 0);
        if (Field_unk3(0, 0) != 0) {
            goto L_0200239c;
        }
        Field_unk57_4(20);
        Field_unk7_3(2, 0x103, 0);
        Field_unk57_4(40);
        Scene_RunPairedStepB(2, 0xe000, 10);
        Scene_RunPairedStepA(2, 10);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0, 10);
            Field_unk58_4(3, 3);
            Scene_RunPairedStepA(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_unk3_4(0, 0x102);
        Field_unk3_4(1, 0x102);
        Field_unk57_4(40);
        Field_unk60_4(1, 2);
        Scene_RunPairedStepA(1, 20);
        Field_unk7_3(1, 0x105, 0);
        Field_unk57_4(120);
        Scene_RunPairedStepA(2, 40);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0x2000, 10);
            Field_unk59_4(3, 4);
            Scene_RunPairedStepA(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_unk57_4(60);
        Field_unk60_4(2, 2);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(2, 0xa000, 40);
            Scene_RunPairedStepB(2, 0xe000, 20);
        }
        Scene_RunPairedStepA(2, 10);
        Field_unk58_4(0, 2);
        Field_unk60_4(1, 2);
        Field_unk57_4(40);
        Scene_RunPairedStepA(2, 20);
        Field_unk59_4(0, 3);
        Field_unk59_4(1, 3);
        Field_unk57_4(20);
        Field_unk61_4(3, 3);
    }
    Field_unk59_4(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Field_unk7_3(2, 0x105, 0);
    Field_unk57_4(40);
    Field_unk59_4(2, 3);
    Field_Do(0x14bf);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0, 10);
        Field_unk58_4(3, 1);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk3_4(1, 0x102);
    Field_unk3_4(0, 0x102);
    Field_unk57_4(40);
    Field_unk60_4(1, 2);
    Scene_RunPairedStepA(1, 20);
    Field_unk7_3(2, 0x105, 0);
    Field_unk57_4(80);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0x2000, 20);
        Field_unk61_4(3, 4);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk60_4(2, 2);
    Field_unk57_4(20);
    Scene_RunPairedStepA(2, 20);
    Field_unk58_4(1, 2);
    Field_unk60_4(0, 2);
    Field_unk57_4(40);
    Scene_RunPairedStepA(2, 20);
    L_020024a0:;
    Field_unk64_4(17);
    Field_unk4_3(1, 0x13333, 0x9999);
    Field_unk4_3(2, 0x13333, 0x9999);
    Field_unk4_3(3, 0x13333, 0x9999);
    Field_unk61_4(1, 2);
    record = Pointer1(Field_unk45_4, 0);
    if ((s32)record != 0) {
        Field_unk72_4(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_unk73_4(1);
    Field_unk53_4(1, 0, 0);
    Field_unk61_4(2, 2);
    record = Pointer1(Field_unk45_4, 0);
    if ((s32)record != 0) {
        Field_unk72_4(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_unk73_4(2);
    Field_unk53_4(2, 0, 0);
    Field_unk53_4(17, 0, 0);
    Field_unk53_4(18, 0, 0);
    if (gOv14 != 0) {
        Field_unk61_4(3, 2);
        record = Pointer1(Field_unk45_4, 0);
        if ((s32)record != 0) {
            Field_unk72_4(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Field_unk73_4(3);
        Field_unk53_4(3, 0, 0);
    }
    Field_unk6(0x843);
    Field_unk74_4();
    Field_unk75_4();
}
