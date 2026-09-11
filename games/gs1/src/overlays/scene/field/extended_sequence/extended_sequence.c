#include "types.h"
#include "resource_38f.h"
#include "object_runtime.h"

/* overlays/scene/field/extended_sequence/extended_actor_sequence_scene.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SCENE_POINTER (*(u8 *volatile *)0x03001ebc)
#define State_FlushPendingWordB698 Func_02000030
#define Effect_AdvanceCounterAndSwitchMode Func_02000050
#define SceneData_InitRecordTable Func_020000a0
#define Effect_UpdateFallingObject Func_020000cc
#define Actor_TurnTowardTarget Func_02000114
#define SceneData_SelectTableAe60BySelector Func_0200016c
#define SceneData_GetTableB010ForSelector26 Func_020001ac
#define SceneData_GetTableB040 Func_020001d4
#define SceneData_SelectTableB080BySelector Func_020001dc
#define Scene_RunActor16MessageBranch Func_02000230
#define Scene_RunActor27Step Func_02000274
#define SceneData_SelectTableB3b0BySelector Func_02000284
#define Scene_ConfigureActor0ThenRun Func_020002b4
#define Scene_SetupEntryBySelector Func_02000694
#define Actor_SetActors19To22HeightByFrameParity Func_02000800
#define Scene_StartEffect141Sequence Func_020008ac
#define Scene_RunSequenceA Func_020008d8
#define Scene_RunPairedStepA Func_020025a8
#define Scene_RunPairedStepB Func_020025c0
#define Effect_AdvanceAngleUntilIdle Func_020025d8
#define Effect_SpawnObject26EveryEightFrames Func_02002608
#define Effect_SetModeByFrameBit1 Func_020026a4
#define Scene_RunFlag845And847Branches Func_02002910


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

struct Ent_02000800 {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Obj_020025d8 {
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

struct Obj_02002608 {
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

extern s16 Data_02000240[];
extern u8 Value_00000027;
extern u8 Value_00000026;
extern u8 Data_0200af80[];
extern u8 Data_0200afc8[];
extern u8 Data_0200ae60[];
extern u8 Data_0200b010[];
extern u8 Value_00000024;
extern u8 Data_0200b098[];
extern u8 Data_0200b368[];
extern u8 Data_0200b080[];
extern u8 Data_0200b590[];
extern u8 Data_0200b3b0[];
extern u8 Data_03001ebc[];
extern u8 Data_0200add8[];
extern u8 Data_0200b2d8[];
extern s32 Data_03001e40;
extern s32 Data_0200b6a0;
extern u8 Data_0200b5d8[];

s32 Func_02002a02(s32, s32);
s32 Func_02002a0a(struct Obj *);
void Func_02002b6e(struct Obj *, s32);
void Func_02002b78(struct Obj *, s32);
void Func_02002ae2(struct Obj *);
u16 Func_02002af2(s32, s32);
s32 Func_02002c10(s32);
void Func_0200029e(s32);
struct Rec *Func_02002c8a(s32);
void Func_02002c70(struct Rec *);
void Func_02002e10(s32);
void Func_02002d58(s32);
void Func_02002d78(s32, s32);
void Func_02002c9c(void);
void Func_02002e3a(s32, s32, s32);
s32 Func_02002d10();
void Func_02002d34();
void Func_02002d88();
void Func_02002d94();
void Func_02002e54();
void Func_02002fbc(void);
s32 Func_0200311e();
void Func_020030dc();
s32 Func_0200312a();
void Func_020030e8();
s32 Func_02003136();
void Func_020030f4();
s32 Func_02003142();
void Func_02003100();
void Func_0200316a();
void Func_02003172();
void Func_02003172_a();
void Func_0200317a();
void Func_02003182();
s32 Func_02003138();
s32 Func_0200317c();
void Func_0200313c();
void Func_0200314c();
void Func_0200315e();
s32 Func_02003190();
void Func_02001078(void);
s32 Func_020031ae();
void Func_020031f8();
void Func_020031fe();
void Func_02003204();
void Func_0200320a();
void Func_02003210();
void Func_02003216();
void Func_02003216_a();
void Func_0200321c();
void Func_02003222();
void Func_02003228();
struct Ent *Func_0200325a(s32);
struct Ent *Func_02003282(s32);
struct Ent *Func_020032aa(s32);
struct Ent *Func_020032d2(s32);
void Func_02003444(s32, s32);
void Func_02003454(s32, s32);
void Func_02003470(void);
void Func_0200344e(s32);
void Func_0200326c(s32);
void Func_02003462(s32);
void Func_0200347e(void);
void Func_0200348a(void);
void Func_020050c4(s32, s32);
void Func_02004fe2(s32);
void Func_020050ec(s32, s32, s32);
void Func_02004ffa(s32);
void Func_02004fd6(struct Obj *);
void Func_020051ee(s32);
struct Obj *Func_02005006(s32, s32, s32, s32);
void Func_02005036(struct Obj *, s32);
void Func_0200506c(struct Obj *, s32, s32, s32);
void Func_02005054(struct Obj *, u8 *);
void Func_020051ac(s32, s32);
void Func_020051b4(s32, s32);
s32 Func_02005334(s32);
void Func_020053da(s32, s32, s32);
void Func_0200533e(s32, s32, s32, s32, s32, s32);
void Func_02005322(void);
void Func_020052e0(s32);
void Func_02005404(s32, s32, s32);
s32 Func_02005372(s32);
void Func_0200536a(s32, s32, s32, s32, s32, s32);
void Func_0200536a_b(void);
void Func_02005386(s32, s32, s32, s32, s32, s32);
void Func_02005328(s32);

/* Each Func_ symbol above names the call word the image holds before loader
 * relocation, not a runtime address. */

/* Calls spelled through this wrapper pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is shared with later uses in the same
 * block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Advance the scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 State_FlushPendingWordB698(s32 arg0)
{
    if (*(s32 *)0x0200B698 != 0) {
        Func_02002a02(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}

s32 Effect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Func_02002a0a(p);
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Func_02002b6e(p, 7);
    } else {
        Func_02002b78(p, 10);
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
            Func_02002ae2(p);
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
        h = Func_02002af2(q->f10 - p->f10, q->f08 - p->f08);
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
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200af80;
    }
    if (v == (s32)&Value_00000026) {
        return (s32)Data_0200afc8;
    }
    return (s32)Data_0200ae60;
}

s32 SceneData_GetTableB010ForSelector26(void)
{
    if (Data_02000240[224] == (s32)&Value_00000026) {
        return (s32)Data_0200b010;
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
    s32 v = Data_02000240[224];
    if (v == (s32)&Value_00000024) {
        if (Func_02002c10(0x845) == 0) {
            Func_0200029e((s32)Data_0200b098);
        }
        return (s32)Data_0200b098;
    }
    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200b368;
    }
    return (s32)Data_0200b080;
}

void Scene_RunActor16MessageBranch(void)
{
    struct Rec *q = Func_02002c8a(0);
    s32 v = q->f06;
    Func_02002c70(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_02002e10(16);
    } else {
        Func_02002d58(0x16b3);
        Func_02002d78(16, 0);
    }
    Func_02002c9c();
}

void Scene_RunActor27Step(void)
{
    Func_02002e3a(27, 0, 1);
}

s32 SceneData_SelectTableB3b0BySelector(void)
{
    if (Data_02000240[224] == (s32)&Value_00000027) {
        return (s32)Data_0200b590;
    }
    return (s32)Data_0200b3b0;
}

void Scene_ConfigureActor0ThenRun(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Func_02002d10(0) + 85) = 0;
    Call3(Func_02002d34, 0, 0x8000, 0x4000);
    Func_02002d94(0, 2);
    Call3(Func_02002d88, 0, 0, -8);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_02002e54(a0);
}

s32 Scene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;

    step = Data_02000240[224];

    if (step == (s32)&Value_00000027) {
        Func_02002fbc();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Func_020030dc(Func_0200311e(23), 0);
    Func_020030e8(Func_0200312a(24), 0);
    Func_020030f4(Func_02003136(25), 0);
    Func_02003100(Func_02003142(26), 0);

    tbl = Data_0200add8;
    Func_0200316a(23, tbl);
    Func_02003172(24, tbl);
    Func_0200317a(25, tbl);
    Func_02003182(26, tbl);

    if (Func_02003138(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Func_0200313c(Func_0200317c(actor), 0);
        }
        Func_0200314c(13, 9, 1, 1, 13, 8);
        Func_0200315e(13, 9, 1, 1, 15, 8);
        {
            s32 m = 14;
            s32 n = 9;
            Func_02003172_a(13, 9, 1, 1, m, n);
        }
    }

    if (Func_02003190(0x843) == 0) {
        if (Data_02000240[225] == 1) {
            Func_02001078();
        }
    }

    if (Func_020031ae(0x843) != 0) {
        Func_020031f8(1);
        Func_020031fe(2);
        Func_02003204(3);
        Func_0200320a(17);
        Func_02003210(18);
        Func_02003216(19);
        Func_0200321c(20);
        Func_02003222(21);
        Func_02003228(22);
        Func_02003216_a(Data_0200b2d8);
    }

    return 0;
}

void Actor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent_02000800 *p;

    p = Func_0200325a(19);
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
    p = Func_02003282(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Func_020032aa(21);
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
    p = Func_020032d2(22);
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
    Func_02003444(141, 1);
    Func_02003454(arg0, arg1);
    Func_02003470();
    Func_0200344e(1);
    Func_0200326c(1);
}

void Scene_RunSequenceA(void)
{
    Func_02003462(2);
    Func_0200347e();
    Func_0200348a();
}

void Scene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Func_020050c4(arg0, 0);
    Func_02004fe2(arg1);
}

void Scene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Func_020050ec(arg0, arg1, 0);
    Func_02004ffa(arg2);
}

s32 Effect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Func_02004fd6(p);
    }
    return 1;
}

void Effect_SpawnObject26EveryEightFrames(void)
{
    struct Obj_02002608 *p;
    struct Sub *q;
    s32 f;
    s32 v;
    s32 w;
    s32 c1 = 0x620000;
    s32 c2 = 0x690000;
    s32 c3 = 0x620000;
    s32 c4 = 0x010d0000;

    f = Data_03001e40 & 7;
    if (f != 0) {
        return;
    }
    if (Data_0200b6a0 != 0) {
        Func_020051ee(200);
    }
    p = Func_02005006(26, c1, 0, c2);
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
    Func_02005036(p, 2);
    Func_0200506c(p, c3, 0, c4);
    Func_02005054(p, Data_0200b5d8);
}

s32 Effect_SetModeByFrameBit1(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_020051ac(arg0, 10);
    } else {
        Func_020051b4(arg0, 7);
    }
    return 0;
}

void Scene_RunFlag845And847Branches(void)
{
    if (Func_02005334(0x845) == 0) {
        Func_020053da(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Func_0200533e(9, 17, 5, 1, k5, k6); }
        Func_02005322();
        Func_020052e0(1);
    } else {
        Func_02005404(9, 0, 0);
    }
    if (Func_02005372(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Func_0200536a(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Func_02005386(23, 11, 5, 7, k5, k6); }
        Func_0200536a_b();
        Func_02005328(1);
    }
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef SCENE_POINTER
#undef State_FlushPendingWordB698
#undef Effect_AdvanceCounterAndSwitchMode
#undef SceneData_InitRecordTable
#undef Effect_UpdateFallingObject
#undef Actor_TurnTowardTarget
#undef SceneData_SelectTableAe60BySelector
#undef SceneData_GetTableB010ForSelector26
#undef SceneData_GetTableB040
#undef SceneData_SelectTableB080BySelector
#undef Scene_RunActor16MessageBranch
#undef Scene_RunActor27Step
#undef SceneData_SelectTableB3b0BySelector
#undef Scene_ConfigureActor0ThenRun
#undef Scene_SetupEntryBySelector
#undef Actor_SetActors19To22HeightByFrameParity
#undef Scene_StartEffect141Sequence
#undef Scene_RunSequenceA
#undef Scene_RunPairedStepA
#undef Scene_RunPairedStepB
#undef Effect_AdvanceAngleUntilIdle
#undef Effect_SpawnObject26EveryEightFrames
#undef Effect_SetModeByFrameBit1
#undef Scene_RunFlag845And847Branches

/* overlays/scene/field/extended_sequence/run_extended_actor_sequence.c */
#define Scene_RunExtendedActorSequence Func_020008ec

void Func_02002a34();
s32 Func_02002a1c();
struct ObjectRuntime *Func_02002a54();
void Func_02002a0c();
void Func_02002b4c();
void Func_020029e4();
void Func_02002ab4();
void Func_0200299c();
void Func_02002b74();
void Func_02002b7c();
void Func_02002a64();
void Func_02002a9c();
void Func_02002a6c();
void Func_02002a74();
void Func_02002b24();
void Func_02002ae4();
void Func_02002a2c();
void Func_02002ad4();
void Func_02002b04();
void Func_020025a8();
void Func_02002ac4();
void Func_02002adc();
void Func_02002abc();
void Func_020008ac();
void Func_020008d8();
void Func_02002b34();
void Func_02002aec();
void Func_02002b3c();
void Func_020025c0();
s32 Func_02002b0c();
s32 Func_02002a4c();
void Func_02002bcc();
void Func_02002b64();
void Func_02002b6c();
s32 Func_020029a4();
void Func_02002aa4();
s32 Func_020029ac();
void Func_02002b2c();
void Func_02002a7c();
void Func_02002a84();
void Func_02002acc();
void Func_02002b14();
void Func_02002a8c();
void Func_02002aac();
s32 Func_02002a24();
void Func_02002bb4();
void Func_02002a3c();

/* The inline adapters retain argument lifetimes at constant-valued calls.
 * Their return types follow the target helpers. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ struct ObjectRuntime *Pointer1(struct ObjectRuntime *(*f)(), s32 a0)
{
    return f(a0);
}

extern s32 Data_0200b69c;

extern s32 Data_0200b68c;
extern s32 Data_0200b690;
extern s32 Data_0200b694;
extern s32 Data_0200b698;
extern s32 Data_0200b6a0;
extern u8 Data_02008801[];
extern u8 Data_0200a609[];
extern u8 Data_0200a7ad[];
extern u8 Data_0200abd4[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ac3c[];
extern u8 Data_0200ac70[];
extern u8 Data_0200acfc[];
extern u8 Data_0200ad20[];
extern u8 Data_0200ad7c[];

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

    Func_02002a34();
    flag_addr = (s32)&Data_0200b69c;
    *(s32 *)flag_addr = Value1(Func_02002a1c, 3);
    record = Func_02002a54(19);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(20);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(21);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(22);
    Func_02002a0c((s32)record, 0);
    Call4(Func_02002b4c, 0x680000, -1, 0x1000000, 0);
    Func_020029e4();
    Call3(Func_02002ab4, 0, 0x170000, 0xf70000);
    Func_0200299c(1);
    Func_02002b74();
    Func_02002b7c();
    Call3(Func_02002a64, 0, 0xcccc, 0x6666);
    Func_02002a9c(0, 121, 238);
    Call3(Func_02002a64, 1, 0x9999, 0x4ccc);
    Call3(Func_02002a64, 2, 0x9999, 0x4ccc);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002ab4(1, record->x, record->z);
    }
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002ab4(2, record->x, record->z);
    }
    Call2(Func_02002a6c, 1, (s32)Data_0200abd4);
    Func_02002a6c(2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002a64, 3, 0x9999, 0x4ccc);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Func_02002ab4(3, record->x, record->z);
        }
        Func_02002a6c(3, (s32)Data_0200ac3c);
    }
    Func_02002a74(2);
    Call3(Func_02002b24, 0, 0xe000, 20);
    Call3(Func_02002b24, 2, 0xe000, 20);
    Call3(Func_02002b24, 1, 0x2000, 20);
    flag_addr = (s32)&Data_0200b69c;
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0x2000, 0);
    }
    Call3(Func_02002b24, 0, 0x2000, 20);
    Call3(Func_02002b24, 2, 0x2000, 20);
    Call3(Func_02002b24, 1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0xe000, 0);
    }
    Call3(Func_02002b24, 0, 0x8000, 0);
    Func_02002ae4(2, 0, 0);
    Func_02002ae4(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 0, 0);
    }
    Func_02002a2c(40);
    Func_02002ad4(1, 1);
    Call1(Func_02002b04, 0x1473);
    Func_020025a8(1, 20);
    Func_02002ae4(0, 1, 0);
    Func_02002ac4(0, 3);
    Func_02002a2c(30);
    Call3(Func_02002a9c, 2, 72, 0x11e);
    Call3(Func_02002a9c, 2, 72, 0x12e);
    Call3(Func_02002a9c, 2, 88, 0x136);
    Func_02002adc(2, 1);
    Func_02002ae4(2, 0, 0);
    Func_02002a2c(20);
    Func_02002ae4(0, 2, 0);
    Func_02002ae4(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 2, 0);
    }
    Func_02002a2c(30);
    Func_02002abc(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Func_02002abc(3, 3);
    }
    Func_02002ac4(0, 3);
    Func_02002a2c(20);
    Func_02002ac4(2, 3);
    Func_02002a2c(30);
    Func_02002b24(2, 0, 0);
    Func_02002a2c(20);
    Func_020008ac(2, 9);
    Func_02002a2c(40);
    Func_020008d8();
    Call3(Func_02002b34, 2, 0x100, 40);
    Call3(Func_02002a64, 2, 0x10000, 0x8000);
    Func_02002a54(2)->action_flags &= 254;
    Call3(Func_02002a9c, 2, 80, 0x136);
    mask = 1;
    Func_02002a2c(1);
    Func_02002a54(2)->action_flags |= mask;
    Call3(Func_02002b34, 1, 0x102, 40);
    Func_020025a8(1, 20);
    Call3(Func_02002b34, 2, 0x102, 40);
    Func_020025a8(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 0, 0);
    }
    Func_02002aec(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0xc000, 0);
    }
    Call3(Func_02002b24, 1, 0xc000, 0);
    Call3(Func_02002b24, 0, 0xe000, 0);
    Func_02002ad4(1, 1);
    Func_020025a8(1, 20);
    Call3(Func_02002a64, 2, 0x8000, 0x4000);
    Func_02002a54(2)->action_flags &= 254;
    Call3(Func_02002a9c, 2, 72, 0x11e);
    Func_02002a2c(1);
    Func_02002a54(2)->action_flags |= mask;
    Call2(Func_02002a6c, 2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b34, 3, 0x105, 0);
        Func_02002a2c(60);
        Func_020025a8(3, 20);
        Func_02002abc(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002ae4(2, 0, 0);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002ac4(2, 3);
    Func_02002a2c(20);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(10);
    Func_020025c0(1, 0x2000, 10);
    Func_020025c0(0, 0xa000, 10);
    Value2(Func_02002b0c, 1, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002a2c(20);
        Func_02002abc(1, 3);
    } else {
        Func_02002ac4(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020025a8(1, 40);
    Func_020025c0(2, 0x2000, 40);
    Func_020025c0(2, 0x8000, 20);
    Func_020025c0(2, 0x4000, 40);
    Call3(Func_02002b34, 2, 0x101, 0);
    Func_02002a2c(60);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 60);
    value = 160;
    Func_020025c0(3, 0x2000, 10);
    Func_02002b24(1, 0x2000, 0);
    Func_020025c0(0, (value << 8), 10);
    Call3(Func_02002b34, 1, 0x101, 0);
    Call3(Func_02002b34, 0, 0x101, 0);
    Func_02002a2c(40);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 10);
    Func_02002adc(1, 2);
    Call1(Func_02002b04, 0x147b);
    Func_020025a8(1, 10);
    Func_020025c0(2, 0xc000, 20);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 20);
    Func_020025c0(1, 0, 20);
    Func_020025c0(0, (value << 8), 40);
    Func_020025c0(1, 0x4000, 20);
    Func_020025c0(0, 0x6000, 30);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(0, 0xe000, 30);
    Func_02002adc(2, 2);
    Call3(Func_02002b34, 2, 0x100, 0);
    Func_02002a2c(40);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 20);
    Func_020025c0(2, 0xc000, 10);
    Func_02002bcc(17);
    Func_02002bcc(206);
    Call2(Func_02002b64, 0x7fff, 0);
    Func_02002b6c(1);
    Func_0200299c(1);
    Data_0200b6a0 = 1;
    Value2(Func_020029a4, (s32)Data_0200a609, 0xc80);
    Func_0200299c(20);
    Call2(Func_02002b64, 0x405210, 1);
    Call2(Func_02002b64, 0x10000, 2);
    Func_02002b6c(120);
    Func_0200299c(60);
    action_a = (s32)Data_0200ac70;
    Func_02002a6c(0, action_a);
    Func_02002a6c(1, action_a);
    Func_02002a6c(2, action_a);
    Func_02002a6c(3, action_a);
    Func_02002a2c(100);
    Func_020025a8(1, 20);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Func_02002a2c(40);
        Call2(Func_02002b3c, 3, 0x102);
        Func_02002a2c(40);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002a2c(20);
    if (Data_0200b69c != 0) {
        value = 128;
        record = Func_02002a54(3);
        record->velocity_y = (value << 10);
        Func_02002a2c(10);
        Func_02002a64(3, (value << 10), (value << 10));
        Call3(Func_02002aa4, 3, -2, 0);
        Call2(Func_02002a6c, 3, (s32)Data_0200acfc);
        record = Func_02002a54(3);
        Func_02002a0c((s32)record, 0);
        Func_02002abc(3, 19);
        Func_02002a2c(10);
    }
    value = 128;
    record = Func_02002a54(0);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Call3(Func_02002a64, 0, (value << 10), (value << 10));
    action_b = (s32)Data_0200acfc;
    Func_02002a6c(0, action_b);
    record = Func_02002a54(0);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(0, 19);
    Func_02002a2c(20);
    record = Pointer1(Func_02002a54, 1);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Call3(Func_02002a64, 1, (value << 10), (value << 10));
    Func_02002a6c(1, action_b);
    record = Func_02002a54(1);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(1, 19);
    Func_02002a2c(40);
    record = Pointer1(Func_02002a54, 2);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Func_02002a6c(2, action_b);
    record = Func_02002a54(2);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(2, 19);
    Data_0200b6a0 = 0;
    Func_02002a2c(160);
    Value1(Func_020029ac, (s32)Data_0200a609);
    Func_02002a2c(120);
    Call2(Func_02002b64, 0x406218, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Data_0200b690 = 0;
    step_addr = (s32)&Data_0200b694;
    Data_0200b68c = 0x800000;
    *(s32 *)step_addr = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Func_02002a2c(180);
    Func_02002bcc(21);
    Func_020025a8(1, 80);
    Func_020025a8(2, 40);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Func_02002a2c(60);
    Func_020025a8(2, 20);
    *(s32 *)step_addr = 2;
    Func_02002ad4(2, 2);
    Func_02002a2c(20);
    Func_02002ad4(1, 1);
    Func_02002a2c(40);
    Func_02002ad4(0, 2);
    Func_02002ad4(3, 1);
    Func_02002a2c(20);
    Func_02002ad4(2, 3);
    Func_02002a2c(40);
    Func_02002ad4(0, 1);
    Func_02002a2c(20);
    Call2(Func_02002ad4, 1, 2);
    Func_02002a2c(20);
    Func_02002ad4(3, 2);
    Call2(Func_02002b3c, 1, 0x102);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Call2(Func_02002b3c, 3, 0x102);
        Func_020025a8(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    work_addr = (s32)&Data_0200b694;
    *(s32 *)work_addr = 3;
    Func_02002a54(0)->unknown_23 &= 254;
    Func_02002a54(1)->unknown_23 &= 254;
    Func_02002a54(2)->unknown_23 &= 254;
    Func_02002a54(3)->unknown_23 &= 254;
    Func_02002b2c(0, 3);
    Func_02002b2c(1, 3);
    Func_02002b2c(2, 3);
    value = 0;
    Func_02002b2c(3, 3);
    Data_0200b698 = value;
    Value2(Func_020029a4, (s32)Data_02008801, 0xc80);
    Func_02002bcc(220);
    Func_02002a54(19)->unknown_23 &= 254;
    Func_02002b2c(19, 2);
    Call3(Func_02002ab4, 19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Func_02002a6c(19, action_c);
    Func_02002a54(20)->unknown_23 &= 254;
    Func_02002b2c(20, 2);
    Call3(Func_02002ab4, 20, 0x640000, 0x1120000);
    Func_02002a6c(20, action_c);
    if (Data_0200b69c != 0) {
        Func_02002a54(21)->unknown_23 &= 254;
        Func_02002b2c(21, 2);
        Call3(Func_02002ab4, 21, 0x4a0000, 0xfe0000);
        Func_02002a6c(21, action_c);
    }
    Func_02002a54(22)->unknown_23 &= 254;
    Func_02002b2c(22, 2);
    Call3(Func_02002ab4, 22, 0x5e0000, 0xe10000);
    Func_02002a6c(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Func_0200299c(1);
        } while (Data_0200b694 != 0);
    }
    Call1(Func_02002a2c, 0x12c);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Func_02002a2c(120);
    Func_02002bcc(17);
    Call2(Func_02002b64, 0x10000, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Func_02002a7c(19);
    Func_02002a7c(20);
    if (Data_0200b69c != 0) {
        Func_02002a7c(21);
    }
    (Func_02002a7c)(22);
    Func_0200299c(1);
    action_d = (s32)Data_0200ad7c;
    (Func_02002a6c)(19, action_d);
    Func_02002a6c(20, action_d);
    if (Data_0200b69c != 0) {
        Func_02002a6c(21, action_d);
    }
    Func_02002a84(22, action_d);
    Func_02002a2c(80);
    Func_02002adc(1, 2);
    Func_02002a2c(40);
    Value2(Func_02002b0c, 1, 0);
    Call3(Func_02002ab4, 17, 0x570000, 0x8b0000);
    Call3(Func_02002ab4, 18, 0x570000, 0x8b0000);
    Func_0200299c(1);
    if (Value2(Func_02002a4c, 17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002adc(0, 1);
    Func_02002a2c(20);
    Call2(Func_02002adc, 2, 2);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_02002adc(3, 2);
        Func_02002a2c(10);
        Call1(Func_02002b04, 0x1488);
        Func_020025a8(3, 40);
    }
    Func_02002ad4(1, 1);
    Call3(Func_02002b34, 1, 0x101, 0);
    Func_02002a2c(80);
    Func_02002adc(2, 2);
    Call1(Func_02002b04, 0x1489);
    Func_020025a8(2, 40);
    Func_02002adc(1, 3);
    Func_02002a2c(40);
    Func_02002b2c(1, 2);
    Func_02002a54(1)->unknown_23 |= 1;
    record = Func_02002a54(1);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(1, 6, 0);
    Func_02002abc(1, 1);
    Func_020025c0(1, 0x4000, 60);
    Func_020025a8(1, 20);
    Func_02002ad4(1, 2);
    (Func_020025a8)(1, 10);
    Func_02002adc(0, 3);
    Func_020025c0(1, 0x2000, 20);
    Call3(Func_02002b34, 1, 0x101, 0);
    Func_02002a2c(40);
    Func_020025c0(1, 0x6000, 40);
    Func_020025c0(1, 0x2000, 20);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(1, 0x2000, 10);
    Func_02002acc(1, 2, 0);
    Func_02002a2c(40);
    Func_02002acc(1, 2, 0);
    Func_02002a2c(10);
    Func_02002acc(1, 4, 0);
    Func_02002a2c(20);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Call3(Func_02002b34, 3, 0x100, 0);
        Func_02002a2c(60);
        Func_02002adc(3, 2);
        Func_02002a2c(80);
        Func_02002b2c(3, 2);
        Func_02002a54(3)->unknown_23 |= 1;
        record = Func_02002a54(3);
        Func_02002a0c((s32)record, 1);
        Func_02002acc(3, 4, 0);
        Call3(Func_02002aa4, 3, -2, 0);
        Func_02002abc(3, 1);
        Func_020025c0(3, 0xe000, 60);
        Func_02002adc(3, 2);
        Func_02002a2c(20);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002acc(1, 2, 0);
    Func_020025c0(1, 0x4000, 20);
    Func_02002ac4(1, 3);
    Func_020025c0(1, 0x2000, 10);
    Func_020025a8(1, 20);
    Func_02002ac4(1, 3);
    Func_02002a2c(10);
    Func_02002adc(2, 1);
    Func_02002a2c(40);
    Func_02002adc(2, 2);
    Func_02002a2c(20);
    value = 1;
    Func_02002b2c(2, 2);
    Func_02002a54(2)->unknown_23 |= value;
    record = Func_02002a54(2);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(2, 4, 0);
    Func_02002abc(2, 1);
    Call3(Func_02002b24, 2, 0xc000, 0);
    Func_02002adc(0, 2);
    Func_02002a2c(10);
    Func_02002b2c(0, 2);
    Func_02002a54(0)->unknown_23 |= value;
    record = Func_02002a54(0);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(0, 4, 0);
    Func_02002abc(0, 1);
    Func_020025c0(0, 0x6000, 60);
    Call3(Func_02002b34, 0, 0x105, 0);
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(60);
    Func_020025c0(0, 0xa000, 20);
    Func_02002ac4(1, 3);
    Func_02002ac4(0, 3);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(1, 0x4000, 10);
    Call3(Func_02002b24, 2, 0xc000, 0);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 20);
    Func_02002abc(0, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(1, 3);
    Func_02002a2c(20);
    Func_02002adc(2, 1);
    Func_02002a2c(20);
    Value2(Func_02002b0c, 2, 0);
    Func_02002abc(2, 3);
    Call3(Func_02002b24, 2, 0xe000, 0);
    Func_02002b24(1, 0x2000, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002abc(2, 3);
        Func_02002ac4(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Func_02002adc(1, 2);
        Func_020025c0(1, 0x2000, 10);
        Func_02002ad4(1, 2);
        Func_02002b14(1, 0);
    }
    Func_020025c0(1, 0x4000, 10);
    Func_02002ac4(1, 4);
    Func_020025a8(1, 20);
    Func_020025c0(2, 0xc000, 10);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 10);
    if (Data_0200b69c != 0) {
        Func_02002adc(3, 2);
        Func_020025c0(3, 0, 20);
        Func_020025c0(3, 0x2000, 10);
        Func_02002abc(3, 4);
        Func_020025a8(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 10);
    Func_02002abc(0, 3);
    Func_02002ac4(1, 3);
    value = 128;
    Func_02002a2c(20);
    Call3(Func_02002b24, 0, 0x6000, 0);
    Func_020025c0(1, (value << 7), 10);
    Func_02002ac4(2, 4);
    Func_02002a2c(20);
    Call3(Func_02002b34, 0, 0x102, 0);
    Call3(Func_02002b34, 1, 0x102, 80);
    Func_020025c0(2, 0xe000, 10);
    Func_02002ad4(2, 2);
    Func_020025a8(2, 20);
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 40);
    Func_02002b24(1, (value << 7), 0);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(2, 0xc000, 10);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 10);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(40);
    Func_020025a8(1, 20);
    Func_02002ac4(2, 3);
    Func_02002a2c(20);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(40);
    Func_020025a8(1, 20);
    Func_02002abc(2, 3);
    Func_020025a8(2, 10);
    Func_02002adc(1, 2);
    Func_020025c0(1, 0x2000, 10);
    Value2(Func_02002b0c, 1, 0);
    Func_02002b24(0, 0xa000, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002ac4(1, 3);
    } else {
        Func_02002a2c(20);
        Func_02002adc(1, 2);
        Func_02002a2c(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002b14(1, 0);
    Func_02002bcc(21);
    Call2(Func_02002b64, 0x406218, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Data_0200b690 = 0;
    Data_0200b68c = 0x800000;
    step_next = (s32)&Data_0200b694;
    *(s32 *)step_next = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Func_02002a2c(80);
    Func_02002ad4(0, 2);
    Call2(Func_02002ad4, 1, 2);
    Func_02002ad4(3, 2);
    Func_02002adc(2, 2);
    Func_02002a2c(60);
    Func_020025c0(2, 0xc000, 10);
    Call1(Func_02002b04, 0x149d);
    Func_020025a8(2, 10);
    Func_020025c0(1, 0xc000, 10);
    Func_020025c0(0, 0xc000, 10);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0xc000, 10);
    }
    Func_02002a54(0)->unknown_23 &= 254;
    Func_02002a54(1)->unknown_23 &= 254;
    Func_02002a54(2)->unknown_23 &= 254;
    Func_02002a54(3)->unknown_23 &= 254;
    Func_02002b2c(0, 3);
    Func_02002b2c(1, 3);
    Func_02002b2c(2, 3);
    Func_02002b2c(3, 3);
    *(s32 *)step_next = 2;
    Func_02002bcc(220);
    Call3(Func_02002ab4, 19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Func_02002a6c(19, action_c);
    Call3(Func_02002ab4, 20, 0x640000, 0x1120000);
    Func_02002a6c(20, action_c);
    if (Data_0200b69c != 0) {
        Call3(Func_02002ab4, 21, 0x4a0000, 0xfe0000);
        Func_02002a6c(21, action_c);
    }
    Call3(Func_02002ab4, 22, 0x5e0000, 0xe10000);
    Call2(Func_02002a6c, 22, action_c);
    Func_02002a2c(120);
    *(s32 *)step_next = 3;
    do {
        Func_0200299c(1);
    } while (Data_0200b694 != 0);
    Func_020025a8(17, 80);
    Func_020025a8(18, 20);
    Call3(Func_02002b34, 0, 0x101, 0);
    Call3(Func_02002b34, 1, 0x101, 0);
    Call3(Func_02002b34, 2, 0x101, 0);
    Call3(Func_02002b34, 3, 0x101, 0);
    Func_02002a2c(60);
    Func_020025a8(18, 20);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 10);
    Call3(Func_02002b34, 0, 0x100, 0);
    Call3(Func_02002b34, 1, 0x100, 0);
    Call3(Func_02002b34, 2, 0x100, 0);
    Call3(Func_02002b34, 3, 0x100, 0);
    Func_02002a2c(40);
    Func_020025a8(17, 10);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(3, 0, 0);
    Func_020025c0(2, 0xc000, 40);
    Func_020025a8(18, 10);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 80);
    Func_020025a8(18, 10);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 40);
    Func_020025a8(17, 10);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Func_02002abc(0, 4);
    Func_02002abc(1, 4);
    Func_02002abc(3, 4);
    Func_02002ac4(2, 4);
    Func_02002a2c(60);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 20);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(18, 10);
    Func_02002ad4(0, 2);
    Func_02002ad4(1, 2);
    Func_02002ad4(3, 2);
    Func_02002adc(2, 2);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_02002b24(3, 0xc000, 0);
    Func_020025a8(18, 20);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(17, 20);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Func_02002a2c(40);
    Func_020025a8(18, 10);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_02002a2c(60);
    Func_02002b14(18, 0);
    Func_02002b14(17, 0);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Func_02002a2c(80);
    Call2(Func_02002b64, 0x10000, 1);
    Func_02002b6c(60);
    Func_0200299c(80);
    Func_02002a7c(19);
    Func_02002a7c(20);
    work_addr = (s32)&Data_0200b69c;
    Func_02002a7c(21);
    Func_02002a7c(22);
    Func_0200299c(1);
    action_d = (s32)Data_0200ad7c;
    Func_02002a6c(19, action_d);
    Func_02002a6c(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Func_02002a6c(21, action_d);
    }
    Func_02002a84(22, action_d);
    Func_02002a2c(20);
    Func_02002b2c(0, 2);
    Func_02002b2c(1, 2);
    Func_02002b2c(2, 2);
    value = 1;
    Func_02002b2c(3, 2);
    Func_02002a54(0)->unknown_23 |= value;
    Func_02002a54(1)->unknown_23 |= value;
    Func_02002a54(2)->unknown_23 |= value;
    Func_02002a54(3)->unknown_23 |= value;
    Func_02002adc(2, 2);
    Func_020025c0(2, 0xe000, 10);
    Value2(Func_02002b0c, 2, 0);
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_02002b24(3, 0, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002adc(1, 2);
        Func_02002a2c(10);
        Value2(Func_02002b0c, 1, 0);
        if (Value2(Func_02002a4c, 0, 0) == 0) {
            Func_020025c0(3, 0, 20);
            Call3(Func_02002b34, 1, 0x101, 0);
            Call3(Func_02002b34, 2, 0x101, 0);
            Call3(Func_02002b34, 3, 0x101, 0);
            Func_02002a2c(40);
            Func_020025c0(1, 0x4000, 20);
            Func_020025a8(1, 10);
            Func_020025c0(2, 0xc000, 20);
            Func_020025c0(2, 0xe000, 20);
            Func_02002ac4(2, 3);
            Func_020025a8(2, 20);
            Func_020025c0(1, 0x2000, 20);
        } else {
            Func_020025c0(3, 0, 20);
            Call3(Func_02002b34, 1, 0x102, 0);
            Call3(Func_02002b34, 2, 0x102, 0);
            Call3(Func_02002b34, 3, 0x102, 40);
            Func_020025c0(1, 0x4000, 20);
            Call1(Func_02002b04, 0x14b4);
            Func_020025a8(1, 20);
            Func_02002ac4(2, 3);
            Func_020025a8(2, 20);
        }
        Func_02002abc(3, 3);
        Func_02002ac4(1, 3);
    } else {
        Func_02002a2c(20);
        Func_02002ac4(1, 3);
        Func_02002a2c(10);
        Call1(Func_02002b04, 0x14b6);
        Func_020025a8(1, 10);
        Call3(Func_02002b24, 1, 0x4000, 0);
        Func_020025c0(0, 0x6000, 20);
        Func_02002abc(1, 3);
        Func_02002ac4(0, 3);
        Func_02002a2c(10);
        Func_02002ac4(2, 4);
        Value2(Func_02002b0c, 2, 0);
        if (Value2(Func_02002a4c, 0, 0) != 0) {
            goto L_0200239c;
        }
        Func_02002a2c(20);
        Call3(Func_02002b34, 2, 0x103, 0);
        Func_02002a2c(40);
        Func_020025c0(2, 0xe000, 10);
        Func_020025a8(2, 10);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0, 10);
            Func_02002ad4(3, 3);
            Func_020025a8(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Call2(Func_02002b3c, 0, 0x102);
        Call2(Func_02002b3c, 1, 0x102);
        Func_02002a2c(40);
        Func_02002adc(1, 2);
        Func_020025a8(1, 20);
        Call3(Func_02002b34, 1, 0x105, 0);
        Func_02002a2c(120);
        Func_020025a8(2, 40);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0x2000, 10);
            Func_02002ac4(3, 4);
            Func_020025a8(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Func_02002a2c(60);
        Func_02002adc(2, 2);
        if (Data_0200b69c != 0) {
            Func_020025c0(2, 0xa000, 40);
            Func_020025c0(2, 0xe000, 20);
        }
        Func_020025a8(2, 10);
        Func_02002ad4(0, 2);
        Func_02002adc(1, 2);
        Func_02002a2c(40);
        Func_020025a8(2, 20);
        Func_02002ac4(0, 3);
        Func_02002ac4(1, 3);
        Func_02002a2c(20);
        Func_02002abc(3, 3);
    }
    Func_02002ac4(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(40);
    Func_02002ac4(2, 3);
    Call1(Func_02002b04, 0x14bf);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0, 10);
        Func_02002ad4(3, 1);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 0, 0x102);
    Func_02002a2c(40);
    Func_02002adc(1, 2);
    Func_020025a8(1, 20);
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(80);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0x2000, 20);
        Func_02002abc(3, 4);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002adc(2, 2);
    Func_02002a2c(20);
    Func_020025a8(2, 20);
    Func_02002ad4(1, 2);
    Func_02002adc(0, 2);
    Func_02002a2c(40);
    Func_020025a8(2, 20);
    L_020024a0:;
    Func_02002bcc(17);
    Call3(Func_02002a64, 1, 0x13333, 0x9999);
    Call3(Func_02002a64, 2, 0x13333, 0x9999);
    Call3(Func_02002a64, 3, 0x13333, 0x9999);
    Func_02002abc(1, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002a8c(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_02002aac(1);
    Func_02002ab4(1, 0, 0);
    Func_02002abc(2, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002a8c(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_02002aac(2);
    Func_02002ab4(2, 0, 0);
    Func_02002ab4(17, 0, 0);
    Func_02002ab4(18, 0, 0);
    if (Data_0200b69c != 0) {
        Func_02002abc(3, 2);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Func_02002a8c(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02002aac(3);
        Func_02002ab4(3, 0, 0);
    }
    Value1(Func_02002a24, 0x843);
    Func_02002bb4();
    Func_02002a3c();
}
#undef Scene_RunExtendedActorSequence
