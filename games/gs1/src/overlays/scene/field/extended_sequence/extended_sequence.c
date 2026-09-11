#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/extended_sequence/extended_sequence.h"
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

struct Rec *Field_Run7(s32);

struct Ent *Field_Run8(s32);
struct Ent *Field_Run9(s32);
struct Ent *Field_Run10(s32);
struct Ent *Field_Run11(s32);

struct Obj *Field_Run12(s32, s32, s32, s32);

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
    s32 v = Field_Check7(p);
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
            Field_Do3(p);
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
        if (Field_Check8(0x845) == 0) {
            Field_Do4((s32)gOv5);
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
    struct Rec *q = Field_Run7(0);
    s32 v = q->f06;
    Field_Do5(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Field_Do6(16);
    } else {
        Field_Do7(0x16b3);
        Field_Apply5(16, 0);
    }
    Field_Run13();
}

void Scene_RunActor27Step(void)
{
    Field_Place9(27, 0, 1);
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

    *(u8 *)(Field_Check9(0) + 85) = 0;
    Field_Place(0, 0x8000, 0x4000);
    Field_Run14(0, 2);
    Field_Place2(0, 0, -8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Field_Run15(a0);
}

s32 Scene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;

    step = gCell[224];

    if (step == (s32)&Value_00000027) {
        Field_Run16();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Field_Run17(Field_Check10(23), 0);
    Field_Run18(Field_Check11(24), 0);
    Field_Run19(Field_Check12(25), 0);
    Field_Run20(Field_Check13(26), 0);

    tbl = gOv10;
    Field_Run21(23, tbl);
    Field_Run22(24, tbl);
    Field_Run23(25, tbl);
    Field_Run24(26, tbl);

    if (Field_Check14(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Field_Run25(Field_Check15(actor), 0);
        }
        Field_Run26(13, 9, 1, 1, 13, 8);
        Field_Run27(13, 9, 1, 1, 15, 8);
        {
            s32 m = 14;
            s32 n = 9;
            Field_Run28(13, 9, 1, 1, m, n);
        }
    }

    if (Field_Check16(0x843) == 0) {
        if (gCell[225] == 1) {
            Field_Run29();
        }
    }

    if (Field_Check17(0x843) != 0) {
        Field_Run30(1);
        Field_Run31(2);
        Field_Run32(3);
        Field_Run33(17);
        Field_Run34(18);
        Field_Run35(19);
        Field_Run36(20);
        Field_Run37(21);
        Field_Run38(22);
        Field_Run39(gOv11);
    }

    return 0;
}

void Actor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent *p;

    p = Field_Run8(19);
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
    p = Field_Run9(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Field_Run10(21);
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
    p = Field_Run11(22);
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
    Field_Run40();
    Field_Do8(1);
    Field_Do9(1);
}

void Scene_RunSequenceA(void)
{
    Field_Do10(2);
    Field_Run41();
    Field_Run42();
}

void Scene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Field_Apply8(arg0, 0);
    Field_Do11(arg1);
}

void Scene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Field_Place10(arg0, arg1, 0);
    Field_Do12(arg2);
}

s32 Effect_AdvanceAngleUntilIdle(struct Obj *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Field_Do13(p);
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
        Field_Do14(200);
    }
    p = Field_Run12(26, c1, 0, c2);
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
    if (Field_Check18(0x845) == 0) {
        Field_Place11(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Field_SetRect(9, 17, 5, 1, k5, k6); }
        Field_Run43();
        Field_Do15(1);
    } else {
        Field_Place12(9, 0, 0);
    }
    if (Field_Check19(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Field_SetRect2(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Field_SetRect3(23, 11, 5, 7, k5, k6); }
        Field_Run44();
        Field_Do16(1);
    }
}

/* overlays/scene/field/extended_sequence/run_extended_actor_sequence.c */

struct ObjectRuntime *Field_Run45();

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

    Field_Run46();
    flag_addr = (s32)&gOv14;
    *(s32 *)flag_addr = Field_Check(3);
    record = Field_Run45(19);
    Field_Run47((s32)record, 0);
    record = Field_Run45(20);
    Field_Run47((s32)record, 0);
    record = Field_Run45(21);
    Field_Run47((s32)record, 0);
    record = Field_Run45(22);
    Field_Run47((s32)record, 0);
    Field_Run(0x680000, -1, 0x1000000, 0);
    Field_Run48();
    Field_Place3(0, 0x170000, 0xf70000);
    Field_Run49(1);
    Field_Run50();
    Field_Run51();
    Field_Place4(0, 0xcccc, 0x6666);
    Field_Run52(0, 121, 238);
    Field_Place4(1, 0x9999, 0x4ccc);
    Field_Place4(2, 0x9999, 0x4ccc);
    record = Pointer1(Field_Run45, 0);
    if ((s32)record != 0) {
        Field_Run53(1, record->x, record->z);
    }
    record = Pointer1(Field_Run45, 0);
    if ((s32)record != 0) {
        Field_Run53(2, record->x, record->z);
    }
    Field_Run2(1, (s32)gOv22);
    Field_Run54(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_Place4(3, 0x9999, 0x4ccc);
        record = Pointer1(Field_Run45, 0);
        if ((s32)record != 0) {
            Field_Run53(3, record->x, record->z);
        }
        Field_Run54(3, (s32)gOv24);
    }
    Field_Run55(2);
    Field_Place5(0, 0xe000, 20);
    Field_Place5(2, 0xe000, 20);
    Field_Place5(1, 0x2000, 20);
    flag_addr = (s32)&gOv14;
    if (*(s32 *)flag_addr != 0) {
        Field_Place5(3, 0x2000, 0);
    }
    Field_Place5(0, 0x2000, 20);
    Field_Place5(2, 0x2000, 20);
    Field_Place5(1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_Place5(3, 0xe000, 0);
    }
    Field_Place5(0, 0x8000, 0);
    Field_Run56(2, 0, 0);
    Field_Run56(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_Run56(3, 0, 0);
    }
    Field_Run57(40);
    Field_Run58(1, 1);
    Field_Do(0x1473);
    Scene_RunPairedStepA(1, 20);
    Field_Run56(0, 1, 0);
    Field_Run59(0, 3);
    Field_Run57(30);
    Field_Place6(2, 72, 0x11e);
    Field_Place6(2, 72, 0x12e);
    Field_Place6(2, 88, 0x136);
    Field_Run60(2, 1);
    Field_Run56(2, 0, 0);
    Field_Run57(20);
    Field_Run56(0, 2, 0);
    Field_Run56(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_Run56(3, 2, 0);
    }
    Field_Run57(30);
    Field_Run61(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Field_Run61(3, 3);
    }
    Field_Run59(0, 3);
    Field_Run57(20);
    Field_Run59(2, 3);
    Field_Run57(30);
    Field_Run62(2, 0, 0);
    Field_Run57(20);
    Scene_StartEffect141Sequence(2, 9);
    Field_Run57(40);
    Scene_RunSequenceA();
    Field_Place7(2, 0x100, 40);
    Field_Place4(2, 0x10000, 0x8000);
    Field_Run45(2)->action_flags &= 254;
    Field_Place6(2, 80, 0x136);
    mask = 1;
    Field_Run57(1);
    Field_Run45(2)->action_flags |= mask;
    Field_Place7(1, 0x102, 40);
    Scene_RunPairedStepA(1, 20);
    Field_Place7(2, 0x102, 40);
    Scene_RunPairedStepA(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Field_Run56(3, 0, 0);
    }
    Field_Run63(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_Place5(3, 0xc000, 0);
    }
    Field_Place5(1, 0xc000, 0);
    Field_Place5(0, 0xe000, 0);
    Field_Run58(1, 1);
    Scene_RunPairedStepA(1, 20);
    Field_Place4(2, 0x8000, 0x4000);
    Field_Run45(2)->action_flags &= 254;
    Field_Place6(2, 72, 0x11e);
    Field_Run57(1);
    Field_Run45(2)->action_flags |= mask;
    Field_Run2(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_Place7(3, 0x105, 0);
        Field_Run57(60);
        Scene_RunPairedStepA(3, 20);
        Field_Run61(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run56(2, 0, 0);
    Field_Run61(0, 3);
    Field_Run61(1, 3);
    Field_Run59(2, 3);
    Field_Run57(20);
    Field_Run3(1, 0x102);
    Field_Run57(10);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_Check2(1, 0);
    if (Field_Check3(0, 0) == 0) {
        Field_Run57(20);
        Field_Run61(1, 3);
    } else {
        Field_Run59(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Scene_RunPairedStepA(1, 40);
    Scene_RunPairedStepB(2, 0x2000, 40);
    Scene_RunPairedStepB(2, 0x8000, 20);
    Scene_RunPairedStepB(2, 0x4000, 40);
    Field_Place7(2, 0x101, 0);
    Field_Run57(60);
    Field_Run62(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 60);
    value = 160;
    Scene_RunPairedStepB(3, 0x2000, 10);
    Field_Run62(1, 0x2000, 0);
    Scene_RunPairedStepB(0, (value << 8), 10);
    Field_Place7(1, 0x101, 0);
    Field_Place7(0, 0x101, 0);
    Field_Run57(40);
    Field_Run62(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Field_Run60(1, 2);
    Field_Do(0x147b);
    Scene_RunPairedStepA(1, 10);
    Scene_RunPairedStepB(2, 0xc000, 20);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(2, 20);
    Scene_RunPairedStepB(1, 0, 20);
    Scene_RunPairedStepB(0, (value << 8), 40);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Scene_RunPairedStepB(0, 0x6000, 30);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(0, 0xe000, 30);
    Field_Run60(2, 2);
    Field_Place7(2, 0x100, 0);
    Field_Run57(40);
    Field_Run62(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_Run64(17);
    Field_Run64(206);
    Field_Run4(0x7fff, 0);
    Field_Run65(1);
    Field_Run49(1);
    gOv12 = 1;
    Field_Check4((s32)gOv20, 0xc80);
    Field_Run49(20);
    Field_Run4(0x405210, 1);
    Field_Run4(0x10000, 2);
    Field_Run65(120);
    Field_Run49(60);
    action_a = (s32)gOv25;
    Field_Run54(0, action_a);
    Field_Run54(1, action_a);
    Field_Run54(2, action_a);
    Field_Run54(3, action_a);
    Field_Run57(100);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Field_Run57(40);
        Field_Run3(3, 0x102);
        Field_Run57(40);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run57(20);
    if (gOv14 != 0) {
        value = 128;
        record = Field_Run45(3);
        record->velocity_y = (value << 10);
        Field_Run57(10);
        Field_Run66(3, (value << 10), (value << 10));
        Field_Place8(3, -2, 0);
        Field_Run2(3, (s32)gOv26);
        record = Field_Run45(3);
        Field_Run47((s32)record, 0);
        Field_Run61(3, 19);
        Field_Run57(10);
    }
    value = 128;
    record = Field_Run45(0);
    record->velocity_y = (value << 10);
    Field_Run57(10);
    Field_Place4(0, (value << 10), (value << 10));
    action_b = (s32)gOv26;
    Field_Run54(0, action_b);
    record = Field_Run45(0);
    Field_Run47((s32)record, 0);
    Field_Run61(0, 19);
    Field_Run57(20);
    record = Pointer1(Field_Run45, 1);
    record->velocity_y = (value << 10);
    Field_Run57(10);
    Field_Place4(1, (value << 10), (value << 10));
    Field_Run54(1, action_b);
    record = Field_Run45(1);
    Field_Run47((s32)record, 0);
    Field_Run61(1, 19);
    Field_Run57(40);
    record = Pointer1(Field_Run45, 2);
    record->velocity_y = (value << 10);
    Field_Run57(10);
    Field_Run54(2, action_b);
    record = Field_Run45(2);
    Field_Run47((s32)record, 0);
    Field_Run61(2, 19);
    gOv12 = 0;
    Field_Run57(160);
    Field_Check5((s32)gOv20);
    Field_Run57(120);
    Field_Run4(0x406218, 1);
    Field_Run65(60);
    Field_Run49(60);
    gOv16 = 0;
    step_addr = (s32)&gOv17;
    gOv15 = 0x800000;
    *(s32 *)step_addr = 1;
    Field_Check4((s32)gOv21, 0xc80);
    Field_Run57(180);
    Field_Run64(21);
    Scene_RunPairedStepA(1, 80);
    Scene_RunPairedStepA(2, 40);
    Field_Run3(0, 0x102);
    Field_Run3(1, 0x102);
    Field_Run3(2, 0x102);
    Field_Run3(3, 0x102);
    Field_Run57(60);
    Scene_RunPairedStepA(2, 20);
    *(s32 *)step_addr = 2;
    Field_Run58(2, 2);
    Field_Run57(20);
    Field_Run58(1, 1);
    Field_Run57(40);
    Field_Run58(0, 2);
    Field_Run58(3, 1);
    Field_Run57(20);
    Field_Run58(2, 3);
    Field_Run57(40);
    Field_Run58(0, 1);
    Field_Run57(20);
    Field_Run5(1, 2);
    Field_Run57(20);
    Field_Run58(3, 2);
    Field_Run3(1, 0x102);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_Run3(3, 0x102);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    work_addr = (s32)&gOv17;
    *(s32 *)work_addr = 3;
    Field_Run45(0)->unknown_23 &= 254;
    Field_Run45(1)->unknown_23 &= 254;
    Field_Run45(2)->unknown_23 &= 254;
    Field_Run45(3)->unknown_23 &= 254;
    Field_Run67(0, 3);
    Field_Run67(1, 3);
    Field_Run67(2, 3);
    value = 0;
    Field_Run67(3, 3);
    gOv18 = value;
    Field_Check4((s32)gOv19, 0xc80);
    Field_Run64(220);
    Field_Run45(19)->unknown_23 &= 254;
    Field_Run67(19, 2);
    Field_Place3(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_Run54(19, action_c);
    Field_Run45(20)->unknown_23 &= 254;
    Field_Run67(20, 2);
    Field_Place3(20, 0x640000, 0x1120000);
    Field_Run54(20, action_c);
    if (gOv14 != 0) {
        Field_Run45(21)->unknown_23 &= 254;
        Field_Run67(21, 2);
        Field_Place3(21, 0x4a0000, 0xfe0000);
        Field_Run54(21, action_c);
    }
    Field_Run45(22)->unknown_23 &= 254;
    Field_Run67(22, 2);
    Field_Place3(22, 0x5e0000, 0xe10000);
    Field_Run54(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Field_Run49(1);
        } while (gOv17 != 0);
    }
    Field_Do2(0x12c);
    Field_Check5((s32)gOv21);
    Field_Run57(120);
    Field_Run64(17);
    Field_Run4(0x10000, 1);
    Field_Run65(60);
    Field_Run49(60);
    Field_Run68(19);
    Field_Run68(20);
    if (gOv14 != 0) {
        Field_Run68(21);
    }
    (Field_Run68)(22);
    Field_Run49(1);
    action_d = (s32)gOv28;
    (Field_Run54)(19, action_d);
    Field_Run54(20, action_d);
    if (gOv14 != 0) {
        Field_Run54(21, action_d);
    }
    Field_Run69(22, action_d);
    Field_Run57(80);
    Field_Run60(1, 2);
    Field_Run57(40);
    Field_Check2(1, 0);
    Field_Place3(17, 0x570000, 0x8b0000);
    Field_Place3(18, 0x570000, 0x8b0000);
    Field_Run49(1);
    if (Field_Check3(17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run60(0, 1);
    Field_Run57(20);
    Field_Run6(2, 2);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Field_Run60(3, 2);
        Field_Run57(10);
        Field_Do(0x1488);
        Scene_RunPairedStepA(3, 40);
    }
    Field_Run58(1, 1);
    Field_Place7(1, 0x101, 0);
    Field_Run57(80);
    Field_Run60(2, 2);
    Field_Do(0x1489);
    Scene_RunPairedStepA(2, 40);
    Field_Run60(1, 3);
    Field_Run57(40);
    Field_Run67(1, 2);
    Field_Run45(1)->unknown_23 |= 1;
    record = Field_Run45(1);
    Field_Run47((s32)record, 1);
    Field_Run70(1, 6, 0);
    Field_Run61(1, 1);
    Scene_RunPairedStepB(1, 0x4000, 60);
    Scene_RunPairedStepA(1, 20);
    Field_Run58(1, 2);
    (Scene_RunPairedStepA)(1, 10);
    Field_Run60(0, 3);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Field_Place7(1, 0x101, 0);
    Field_Run57(40);
    Scene_RunPairedStepB(1, 0x6000, 40);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_Run70(1, 2, 0);
    Field_Run57(40);
    Field_Run70(1, 2, 0);
    Field_Run57(10);
    Field_Run70(1, 4, 0);
    Field_Run57(20);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_Place7(3, 0x100, 0);
        Field_Run57(60);
        Field_Run60(3, 2);
        Field_Run57(80);
        Field_Run67(3, 2);
        Field_Run45(3)->unknown_23 |= 1;
        record = Field_Run45(3);
        Field_Run47((s32)record, 1);
        Field_Run70(3, 4, 0);
        Field_Place8(3, -2, 0);
        Field_Run61(3, 1);
        Scene_RunPairedStepB(3, 0xe000, 60);
        Field_Run60(3, 2);
        Field_Run57(20);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run70(1, 2, 0);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Field_Run59(1, 3);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepA(1, 20);
    Field_Run59(1, 3);
    Field_Run57(10);
    Field_Run60(2, 1);
    Field_Run57(40);
    Field_Run60(2, 2);
    Field_Run57(20);
    value = 1;
    Field_Run67(2, 2);
    Field_Run45(2)->unknown_23 |= value;
    record = Field_Run45(2);
    Field_Run47((s32)record, 1);
    Field_Run70(2, 4, 0);
    Field_Run61(2, 1);
    Field_Place5(2, 0xc000, 0);
    Field_Run60(0, 2);
    Field_Run57(10);
    Field_Run67(0, 2);
    Field_Run45(0)->unknown_23 |= value;
    record = Field_Run45(0);
    Field_Run47((s32)record, 1);
    Field_Run70(0, 4, 0);
    Field_Run61(0, 1);
    Scene_RunPairedStepB(0, 0x6000, 60);
    Field_Place7(0, 0x105, 0);
    Field_Place7(2, 0x105, 0);
    Field_Run57(60);
    Scene_RunPairedStepB(0, 0xa000, 20);
    Field_Run59(1, 3);
    Field_Run59(0, 3);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_Place5(2, 0xc000, 0);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(2, 20);
    Field_Run61(0, 3);
    Field_Run61(3, 3);
    Field_Run59(1, 3);
    Field_Run57(20);
    Field_Run60(2, 1);
    Field_Run57(20);
    Field_Check2(2, 0);
    Field_Run61(2, 3);
    Field_Place5(2, 0xe000, 0);
    Field_Run62(1, 0x2000, 0);
    if (Field_Check3(0, 0) == 0) {
        Field_Run61(2, 3);
        Field_Run59(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Field_Run60(1, 2);
        Scene_RunPairedStepB(1, 0x2000, 10);
        Field_Run58(1, 2);
        Field_Run71(1, 0);
    }
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_Run59(1, 4);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(2, 10);
    if (gOv14 != 0) {
        Field_Run60(3, 2);
        Scene_RunPairedStepB(3, 0, 20);
        Scene_RunPairedStepB(3, 0x2000, 10);
        Field_Run61(3, 4);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Place5(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_Run61(0, 3);
    Field_Run59(1, 3);
    value = 128;
    Field_Run57(20);
    Field_Place5(0, 0x6000, 0);
    Scene_RunPairedStepB(1, (value << 7), 10);
    Field_Run59(2, 4);
    Field_Run57(20);
    Field_Place7(0, 0x102, 0);
    Field_Place7(1, 0x102, 80);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_Run58(2, 2);
    Scene_RunPairedStepA(2, 20);
    Field_Place5(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 40);
    Field_Run62(1, (value << 7), 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_Run3(1, 0x102);
    Field_Run57(40);
    Scene_RunPairedStepA(1, 20);
    Field_Run59(2, 3);
    Field_Run57(20);
    Field_Run3(1, 0x102);
    Field_Run57(40);
    Scene_RunPairedStepA(1, 20);
    Field_Run61(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_Run60(1, 2);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_Check2(1, 0);
    Field_Run62(0, 0xa000, 0);
    if (Field_Check3(0, 0) == 0) {
        Field_Run59(1, 3);
    } else {
        Field_Run57(20);
        Field_Run60(1, 2);
        Field_Run57(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run71(1, 0);
    Field_Run64(21);
    Field_Run4(0x406218, 1);
    Field_Run65(60);
    Field_Run49(60);
    gOv16 = 0;
    gOv15 = 0x800000;
    step_next = (s32)&gOv17;
    *(s32 *)step_next = 1;
    Field_Check4((s32)gOv21, 0xc80);
    Field_Run57(80);
    Field_Run58(0, 2);
    Field_Run5(1, 2);
    Field_Run58(3, 2);
    Field_Run60(2, 2);
    Field_Run57(60);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_Do(0x149d);
    Scene_RunPairedStepA(2, 10);
    Scene_RunPairedStepB(1, 0xc000, 10);
    Scene_RunPairedStepB(0, 0xc000, 10);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0xc000, 10);
    }
    Field_Run45(0)->unknown_23 &= 254;
    Field_Run45(1)->unknown_23 &= 254;
    Field_Run45(2)->unknown_23 &= 254;
    Field_Run45(3)->unknown_23 &= 254;
    Field_Run67(0, 3);
    Field_Run67(1, 3);
    Field_Run67(2, 3);
    Field_Run67(3, 3);
    *(s32 *)step_next = 2;
    Field_Run64(220);
    Field_Place3(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_Run54(19, action_c);
    Field_Place3(20, 0x640000, 0x1120000);
    Field_Run54(20, action_c);
    if (gOv14 != 0) {
        Field_Place3(21, 0x4a0000, 0xfe0000);
        Field_Run54(21, action_c);
    }
    Field_Place3(22, 0x5e0000, 0xe10000);
    Field_Run2(22, action_c);
    Field_Run57(120);
    *(s32 *)step_next = 3;
    do {
        Field_Run49(1);
    } while (gOv17 != 0);
    Scene_RunPairedStepA(17, 80);
    Scene_RunPairedStepA(18, 20);
    Field_Place7(0, 0x101, 0);
    Field_Place7(1, 0x101, 0);
    Field_Place7(2, 0x101, 0);
    Field_Place7(3, 0x101, 0);
    Field_Run57(60);
    Scene_RunPairedStepA(18, 20);
    Field_Run61(0, 3);
    Field_Run61(1, 3);
    Field_Run61(3, 3);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_Run61(0, 3);
    Field_Run61(1, 3);
    Field_Run61(3, 3);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_Place7(0, 0x100, 0);
    Field_Place7(1, 0x100, 0);
    Field_Place7(2, 0x100, 0);
    Field_Place7(3, 0x100, 0);
    Field_Run57(40);
    Scene_RunPairedStepA(17, 10);
    Field_Place5(0, 0x8000, 0);
    Field_Place5(1, 0x4000, 0);
    Field_Run62(3, 0, 0);
    Scene_RunPairedStepB(2, 0xc000, 40);
    Scene_RunPairedStepA(18, 10);
    Field_Run3(0, 0x102);
    Field_Run3(1, 0x102);
    Field_Run3(2, 0x102);
    Field_Run3(3, 0x102);
    Field_Run62(0, 0xc000, 0);
    Field_Run62(1, 0xc000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 80);
    Scene_RunPairedStepA(18, 10);
    Field_Place5(0, 0x8000, 0);
    Field_Place5(1, 0x4000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 40);
    Scene_RunPairedStepA(17, 10);
    Field_Run62(0, 0xc000, 0);
    Field_Run62(1, 0xc000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Field_Run61(0, 4);
    Field_Run61(1, 4);
    Field_Run61(3, 4);
    Field_Run59(2, 4);
    Field_Run57(60);
    Scene_RunPairedStepA(18, 10);
    Field_Run61(0, 3);
    Field_Run61(1, 3);
    Field_Run61(3, 3);
    Field_Run59(2, 3);
    Scene_RunPairedStepA(18, 20);
    Field_Place5(0, 0x8000, 0);
    Field_Place5(1, 0x4000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(18, 10);
    Field_Run58(0, 2);
    Field_Run58(1, 2);
    Field_Run58(3, 2);
    Field_Run60(2, 2);
    Field_Run62(0, 0xc000, 0);
    Field_Run62(1, 0xc000, 0);
    Field_Run62(2, 0xc000, 0);
    Field_Run62(3, 0xc000, 0);
    Scene_RunPairedStepA(18, 20);
    Field_Place5(0, 0x8000, 0);
    Field_Place5(1, 0x4000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(17, 20);
    Field_Run3(0, 0x102);
    Field_Run3(1, 0x102);
    Field_Run3(3, 0x102);
    Field_Run3(2, 0x102);
    Field_Run57(40);
    Scene_RunPairedStepA(18, 10);
    Field_Run62(0, 0xc000, 0);
    Field_Run62(1, 0xc000, 0);
    Field_Run62(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Scene_RunPairedStepA(18, 10);
    Field_Run61(0, 3);
    Field_Run61(1, 3);
    Field_Run61(3, 3);
    Field_Run59(2, 3);
    Field_Run57(60);
    Field_Run71(18, 0);
    Field_Run71(17, 0);
    Field_Check5((s32)gOv21);
    Field_Run57(80);
    Field_Run4(0x10000, 1);
    Field_Run65(60);
    Field_Run49(80);
    Field_Run68(19);
    Field_Run68(20);
    work_addr = (s32)&gOv14;
    Field_Run68(21);
    Field_Run68(22);
    Field_Run49(1);
    action_d = (s32)gOv28;
    Field_Run54(19, action_d);
    Field_Run54(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Field_Run54(21, action_d);
    }
    Field_Run69(22, action_d);
    Field_Run57(20);
    Field_Run67(0, 2);
    Field_Run67(1, 2);
    Field_Run67(2, 2);
    value = 1;
    Field_Run67(3, 2);
    Field_Run45(0)->unknown_23 |= value;
    Field_Run45(1)->unknown_23 |= value;
    Field_Run45(2)->unknown_23 |= value;
    Field_Run45(3)->unknown_23 |= value;
    Field_Run60(2, 2);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_Check2(2, 0);
    Field_Place5(1, 0x2000, 0);
    Field_Run62(3, 0, 0);
    if (Field_Check3(0, 0) == 0) {
        Field_Run60(1, 2);
        Field_Run57(10);
        Field_Check2(1, 0);
        if (Field_Check3(0, 0) == 0) {
            Scene_RunPairedStepB(3, 0, 20);
            Field_Place7(1, 0x101, 0);
            Field_Place7(2, 0x101, 0);
            Field_Place7(3, 0x101, 0);
            Field_Run57(40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Scene_RunPairedStepA(1, 10);
            Scene_RunPairedStepB(2, 0xc000, 20);
            Scene_RunPairedStepB(2, 0xe000, 20);
            Field_Run59(2, 3);
            Scene_RunPairedStepA(2, 20);
            Scene_RunPairedStepB(1, 0x2000, 20);
        } else {
            Scene_RunPairedStepB(3, 0, 20);
            Field_Place7(1, 0x102, 0);
            Field_Place7(2, 0x102, 0);
            Field_Place7(3, 0x102, 40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Field_Do(0x14b4);
            Scene_RunPairedStepA(1, 20);
            Field_Run59(2, 3);
            Scene_RunPairedStepA(2, 20);
        }
        Field_Run61(3, 3);
        Field_Run59(1, 3);
    } else {
        Field_Run57(20);
        Field_Run59(1, 3);
        Field_Run57(10);
        Field_Do(0x14b6);
        Scene_RunPairedStepA(1, 10);
        Field_Place5(1, 0x4000, 0);
        Scene_RunPairedStepB(0, 0x6000, 20);
        Field_Run61(1, 3);
        Field_Run59(0, 3);
        Field_Run57(10);
        Field_Run59(2, 4);
        Field_Check2(2, 0);
        if (Field_Check3(0, 0) != 0) {
            goto L_0200239c;
        }
        Field_Run57(20);
        Field_Place7(2, 0x103, 0);
        Field_Run57(40);
        Scene_RunPairedStepB(2, 0xe000, 10);
        Scene_RunPairedStepA(2, 10);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0, 10);
            Field_Run58(3, 3);
            Scene_RunPairedStepA(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_Run3(0, 0x102);
        Field_Run3(1, 0x102);
        Field_Run57(40);
        Field_Run60(1, 2);
        Scene_RunPairedStepA(1, 20);
        Field_Place7(1, 0x105, 0);
        Field_Run57(120);
        Scene_RunPairedStepA(2, 40);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0x2000, 10);
            Field_Run59(3, 4);
            Scene_RunPairedStepA(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_Run57(60);
        Field_Run60(2, 2);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(2, 0xa000, 40);
            Scene_RunPairedStepB(2, 0xe000, 20);
        }
        Scene_RunPairedStepA(2, 10);
        Field_Run58(0, 2);
        Field_Run60(1, 2);
        Field_Run57(40);
        Scene_RunPairedStepA(2, 20);
        Field_Run59(0, 3);
        Field_Run59(1, 3);
        Field_Run57(20);
        Field_Run61(3, 3);
    }
    Field_Run59(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Field_Place7(2, 0x105, 0);
    Field_Run57(40);
    Field_Run59(2, 3);
    Field_Do(0x14bf);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0, 10);
        Field_Run58(3, 1);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run3(1, 0x102);
    Field_Run3(0, 0x102);
    Field_Run57(40);
    Field_Run60(1, 2);
    Scene_RunPairedStepA(1, 20);
    Field_Place7(2, 0x105, 0);
    Field_Run57(80);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0x2000, 20);
        Field_Run61(3, 4);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_Run60(2, 2);
    Field_Run57(20);
    Scene_RunPairedStepA(2, 20);
    Field_Run58(1, 2);
    Field_Run60(0, 2);
    Field_Run57(40);
    Scene_RunPairedStepA(2, 20);
    L_020024a0:;
    Field_Run64(17);
    Field_Place4(1, 0x13333, 0x9999);
    Field_Place4(2, 0x13333, 0x9999);
    Field_Place4(3, 0x13333, 0x9999);
    Field_Run61(1, 2);
    record = Pointer1(Field_Run45, 0);
    if ((s32)record != 0) {
        Field_Run72(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_Run73(1);
    Field_Run53(1, 0, 0);
    Field_Run61(2, 2);
    record = Pointer1(Field_Run45, 0);
    if ((s32)record != 0) {
        Field_Run72(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_Run73(2);
    Field_Run53(2, 0, 0);
    Field_Run53(17, 0, 0);
    Field_Run53(18, 0, 0);
    if (gOv14 != 0) {
        Field_Run61(3, 2);
        record = Pointer1(Field_Run45, 0);
        if ((s32)record != 0) {
            Field_Run72(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Field_Run73(3);
        Field_Run53(3, 0, 0);
    }
    Field_Check6(0x843);
    Field_Run74();
    Field_Run75();
}
