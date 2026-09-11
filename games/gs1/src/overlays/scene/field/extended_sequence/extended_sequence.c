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

struct Rec *Field_unk_02002c8a(s32);

struct Ent *Field_unk_0200325a(s32);
struct Ent *Field_unk_02003282(s32);
struct Ent *Field_unk_020032aa(s32);
struct Ent *Field_unk_020032d2(s32);

struct Obj *Field_unk_02005006(s32, s32, s32, s32);

/* Each Func_ symbol above names the call word the image holds before loader
 * relocation, not a runtime address. */

/* Calls spelled through this wrapper pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is shared with later uses in the same
 * block. A value-returning call also sets r0 last of its arguments. */

s32 State_FlushPendingWordB698(s32 arg0)
{
    if (*(s32 *)0x0200B698 != 0) {
        Field_unk_02002a02(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}

s32 Effect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Field_unk_02002a0a(p);
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Field_unk_02002b6e(p, 7);
    } else {
        Field_unk_02002b78(p, 10);
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
            Field_unk_02002ae2(p);
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
        h = Field_unk_02002af2(q->f10 - p->f10, q->f08 - p->f08);
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
        if (Field_unk_02002c10(0x845) == 0) {
            Field_unk_0200029e((s32)gOv5);
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
    struct Rec *q = Field_unk_02002c8a(0);
    s32 v = q->f06;
    Field_unk_02002c70(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Field_unk_02002e10(16);
    } else {
        Field_unk_02002d58(0x16b3);
        Field_unk_02002d78(16, 0);
    }
    Field_unk_02002c9c();
}

void Scene_RunActor27Step(void)
{
    Field_unk_02002e3a(27, 0, 1);
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

    *(u8 *)(Field_unk_02002d10(0) + 85) = 0;
    Field_unk_02002d34(0, 0x8000, 0x4000);
    Field_unk_02002d94(0, 2);
    Field_unk_02002d88(0, 0, -8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Field_unk_02002e54(a0);
}

s32 Scene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;

    step = gCell[224];

    if (step == (s32)&Value_00000027) {
        Field_unk_02002fbc();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Field_unk_020030dc(Field_unk_0200311e(23), 0);
    Field_unk_020030e8(Field_unk_0200312a(24), 0);
    Field_unk_020030f4(Field_unk_02003136(25), 0);
    Field_unk_02003100(Field_unk_02003142(26), 0);

    tbl = gOv10;
    Field_unk_0200316a(23, tbl);
    Field_unk_02003172(24, tbl);
    Field_unk_0200317a(25, tbl);
    Field_unk_02003182(26, tbl);

    if (Field_TestFlag845(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Field_unk_0200313c(Field_unk_0200317c(actor), 0);
        }
        Field_unk_0200314c(13, 9, 1, 1, 13, 8);
        Field_unk_0200315e(13, 9, 1, 1, 15, 8);
        {
            s32 m = 14;
            s32 n = 9;
            Field_unk_02003172_a(13, 9, 1, 1, m, n);
        }
    }

    if (Field_unk_02003190(0x843) == 0) {
        if (gCell[225] == 1) {
            Field_unk_02001078();
        }
    }

    if (Field_unk_020031ae(0x843) != 0) {
        Field_unk_020031f8(1);
        Field_unk_020031fe(2);
        Field_unk_02003204(3);
        Field_unk_0200320a(17);
        Field_unk_02003210(18);
        Field_unk_02003216(19);
        Field_unk_0200321c(20);
        Field_unk_02003222(21);
        Field_unk_02003228(22);
        Field_unk_02003216_a(gOv11);
    }

    return 0;
}

void Actor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent *p;

    p = Field_unk_0200325a(19);
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
    p = Field_unk_02003282(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (*(u32 *)0x03001e40 & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Field_unk_020032aa(21);
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
    p = Field_unk_020032d2(22);
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
    Field_unk_02003444(141, 1);
    Field_unk_02003454(arg0, arg1);
    Field_unk_02003470();
    Field_unk_0200344e(1);
    Field_unk_0200326c(1);
}

void Scene_RunSequenceA(void)
{
    Field_unk_02003462(2);
    Field_unk_0200347e();
    Field_unk_0200348a();
}

void Scene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Field_unk_020050c4(arg0, 0);
    Field_unk_02004fe2(arg1);
}

void Scene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Field_unk_020050ec(arg0, arg1, 0);
    Field_unk_02004ffa(arg2);
}

s32 Effect_AdvanceAngleUntilIdle(struct Obj *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Field_unk_02004fd6(p);
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
        Field_unk_020051ee(200);
    }
    p = Field_unk_02005006(26, c1, 0, c2);
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
    Field_unk_02005036(p, 2);
    Field_SetMode(p, c3, 0, c4);
    Field_unk_02005054(p, gOv13);
}

s32 Effect_SetModeByFrameBit1(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Field_unk_020051ac(arg0, 10);
    } else {
        Field_unk_020051b4(arg0, 7);
    }
    return 0;
}

void Scene_RunFlag845And847Branches(void)
{
    if (Field_unk_02005334(0x845) == 0) {
        Field_unk_020053da(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Field_SetRect(9, 17, 5, 1, k5, k6); }
        Field_unk_02005322();
        Field_unk_020052e0(1);
    } else {
        Field_unk_02005404(9, 0, 0);
    }
    if (Field_TestFlag847(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Field_unk_0200536a(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Field_unk_02005386(23, 11, 5, 7, k5, k6); }
        Field_unk_0200536a_b();
        Field_unk_02005328(1);
    }
}

/* overlays/scene/field/extended_sequence/run_extended_actor_sequence.c */

struct ObjectRuntime *Field_unk_02002a54();

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

    Field_unk_02002a34();
    flag_addr = (s32)&gOv14;
    *(s32 *)flag_addr = Field_Check(3);
    record = Field_unk_02002a54(19);
    Field_unk_02002a0c((s32)record, 0);
    record = Field_unk_02002a54(20);
    Field_unk_02002a0c((s32)record, 0);
    record = Field_unk_02002a54(21);
    Field_unk_02002a0c((s32)record, 0);
    record = Field_unk_02002a54(22);
    Field_unk_02002a0c((s32)record, 0);
    Field_unk_02002b4c(0x680000, -1, 0x1000000, 0);
    Field_unk_020029e4();
    Field_unk_02002ab4(0, 0x170000, 0xf70000);
    Field_unk_0200299c(1);
    Field_unk_02002b74();
    Field_unk_02002b7c();
    Field_unk_02002a64(0, 0xcccc, 0x6666);
    Field_unk_02002a9c(0, 121, 238);
    Field_unk_02002a64(1, 0x9999, 0x4ccc);
    Field_unk_02002a64(2, 0x9999, 0x4ccc);
    record = Pointer1(Field_unk_02002a54, 0);
    if ((s32)record != 0) {
        Field_unk_02002ab4(1, record->x, record->z);
    }
    record = Pointer1(Field_unk_02002a54, 0);
    if ((s32)record != 0) {
        Field_unk_02002ab4(2, record->x, record->z);
    }
    Field_unk_02002a6c(1, (s32)gOv22);
    Field_unk_02002a6c(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002a64(3, 0x9999, 0x4ccc);
        record = Pointer1(Field_unk_02002a54, 0);
        if ((s32)record != 0) {
            Field_unk_02002ab4(3, record->x, record->z);
        }
        Field_unk_02002a6c(3, (s32)gOv24);
    }
    Field_unk_02002a74(2);
    Field_unk_02002b24(0, 0xe000, 20);
    Field_unk_02002b24(2, 0xe000, 20);
    Field_unk_02002b24(1, 0x2000, 20);
    flag_addr = (s32)&gOv14;
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002b24(3, 0x2000, 0);
    }
    Field_unk_02002b24(0, 0x2000, 20);
    Field_unk_02002b24(2, 0x2000, 20);
    Field_unk_02002b24(1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002b24(3, 0xe000, 0);
    }
    Field_unk_02002b24(0, 0x8000, 0);
    Field_unk_02002ae4(2, 0, 0);
    Field_unk_02002ae4(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002ae4(3, 0, 0);
    }
    Field_unk_02002a2c(40);
    Field_unk_02002ad4(1, 1);
    Field_unk_02002b04(0x1473);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002ae4(0, 1, 0);
    Field_unk_02002ac4(0, 3);
    Field_unk_02002a2c(30);
    Field_unk_02002a9c(2, 72, 0x11e);
    Field_unk_02002a9c(2, 72, 0x12e);
    Field_unk_02002a9c(2, 88, 0x136);
    Field_unk_02002adc(2, 1);
    Field_unk_02002ae4(2, 0, 0);
    Field_unk_02002a2c(20);
    Field_unk_02002ae4(0, 2, 0);
    Field_unk_02002ae4(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002ae4(3, 2, 0);
    }
    Field_unk_02002a2c(30);
    Field_unk_02002abc(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002abc(3, 3);
    }
    Field_unk_02002ac4(0, 3);
    Field_unk_02002a2c(20);
    Field_unk_02002ac4(2, 3);
    Field_unk_02002a2c(30);
    Field_unk_02002b24(2, 0, 0);
    Field_unk_02002a2c(20);
    Scene_StartEffect141Sequence(2, 9);
    Field_unk_02002a2c(40);
    Scene_RunSequenceA();
    Field_unk_02002b34(2, 0x100, 40);
    Field_unk_02002a64(2, 0x10000, 0x8000);
    Field_unk_02002a54(2)->action_flags &= 254;
    Field_unk_02002a9c(2, 80, 0x136);
    mask = 1;
    Field_unk_02002a2c(1);
    Field_unk_02002a54(2)->action_flags |= mask;
    Field_unk_02002b34(1, 0x102, 40);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002b34(2, 0x102, 40);
    Scene_RunPairedStepA(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002ae4(3, 0, 0);
    }
    Field_unk_02002aec(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002b24(3, 0xc000, 0);
    }
    Field_unk_02002b24(1, 0xc000, 0);
    Field_unk_02002b24(0, 0xe000, 0);
    Field_unk_02002ad4(1, 1);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002a64(2, 0x8000, 0x4000);
    Field_unk_02002a54(2)->action_flags &= 254;
    Field_unk_02002a9c(2, 72, 0x11e);
    Field_unk_02002a2c(1);
    Field_unk_02002a54(2)->action_flags |= mask;
    Field_unk_02002a6c(2, (s32)gOv23);
    if (*(s32 *)flag_addr != 0) {
        Field_unk_02002b34(3, 0x105, 0);
        Field_unk_02002a2c(60);
        Scene_RunPairedStepA(3, 20);
        Field_unk_02002abc(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002ae4(2, 0, 0);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(1, 3);
    Field_unk_02002ac4(2, 3);
    Field_unk_02002a2c(20);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002a2c(10);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_unk_02002b0c(1, 0);
    if (Field_unk_02002a4c(0, 0) == 0) {
        Field_unk_02002a2c(20);
        Field_unk_02002abc(1, 3);
    } else {
        Field_unk_02002ac4(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Scene_RunPairedStepA(1, 40);
    Scene_RunPairedStepB(2, 0x2000, 40);
    Scene_RunPairedStepB(2, 0x8000, 20);
    Scene_RunPairedStepB(2, 0x4000, 40);
    Field_unk_02002b34(2, 0x101, 0);
    Field_unk_02002a2c(60);
    Field_unk_02002b24(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 60);
    value = 160;
    Scene_RunPairedStepB(3, 0x2000, 10);
    Field_unk_02002b24(1, 0x2000, 0);
    Scene_RunPairedStepB(0, (value << 8), 10);
    Field_unk_02002b34(1, 0x101, 0);
    Field_unk_02002b34(0, 0x101, 0);
    Field_unk_02002a2c(40);
    Field_unk_02002b24(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Field_unk_02002adc(1, 2);
    Field_unk_02002b04(0x147b);
    Scene_RunPairedStepA(1, 10);
    Scene_RunPairedStepB(2, 0xc000, 20);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(2, 20);
    Scene_RunPairedStepB(1, 0, 20);
    Scene_RunPairedStepB(0, (value << 8), 40);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Scene_RunPairedStepB(0, 0x6000, 30);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(0, 0xe000, 30);
    Field_unk_02002adc(2, 2);
    Field_unk_02002b34(2, 0x100, 0);
    Field_unk_02002a2c(40);
    Field_unk_02002b24(1, 0x4000, 0);
    Scene_RunPairedStepB(0, 0x6000, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk_02002bcc(17);
    Field_unk_02002bcc(206);
    Field_unk_02002b64(0x7fff, 0);
    Field_unk_02002b6c(1);
    Field_unk_0200299c(1);
    gOv12 = 1;
    Field_unk_020029a4((s32)gOv20, 0xc80);
    Field_unk_0200299c(20);
    Field_unk_02002b64(0x405210, 1);
    Field_unk_02002b64(0x10000, 2);
    Field_unk_02002b6c(120);
    Field_unk_0200299c(60);
    action_a = (s32)gOv25;
    Field_unk_02002a6c(0, action_a);
    Field_unk_02002a6c(1, action_a);
    Field_unk_02002a6c(2, action_a);
    Field_unk_02002a6c(3, action_a);
    Field_unk_02002a2c(100);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Field_unk_02002a2c(40);
        Field_unk_02002b3c(3, 0x102);
        Field_unk_02002a2c(40);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002a2c(20);
    if (gOv14 != 0) {
        value = 128;
        record = Field_unk_02002a54(3);
        record->velocity_y = (value << 10);
        Field_unk_02002a2c(10);
        Field_unk_02002a64(3, (value << 10), (value << 10));
        Field_unk_02002aa4(3, -2, 0);
        Field_unk_02002a6c(3, (s32)gOv26);
        record = Field_unk_02002a54(3);
        Field_unk_02002a0c((s32)record, 0);
        Field_unk_02002abc(3, 19);
        Field_unk_02002a2c(10);
    }
    value = 128;
    record = Field_unk_02002a54(0);
    record->velocity_y = (value << 10);
    Field_unk_02002a2c(10);
    Field_unk_02002a64(0, (value << 10), (value << 10));
    action_b = (s32)gOv26;
    Field_unk_02002a6c(0, action_b);
    record = Field_unk_02002a54(0);
    Field_unk_02002a0c((s32)record, 0);
    Field_unk_02002abc(0, 19);
    Field_unk_02002a2c(20);
    record = Pointer1(Field_unk_02002a54, 1);
    record->velocity_y = (value << 10);
    Field_unk_02002a2c(10);
    Field_unk_02002a64(1, (value << 10), (value << 10));
    Field_unk_02002a6c(1, action_b);
    record = Field_unk_02002a54(1);
    Field_unk_02002a0c((s32)record, 0);
    Field_unk_02002abc(1, 19);
    Field_unk_02002a2c(40);
    record = Pointer1(Field_unk_02002a54, 2);
    record->velocity_y = (value << 10);
    Field_unk_02002a2c(10);
    Field_unk_02002a6c(2, action_b);
    record = Field_unk_02002a54(2);
    Field_unk_02002a0c((s32)record, 0);
    Field_unk_02002abc(2, 19);
    gOv12 = 0;
    Field_unk_02002a2c(160);
    Field_unk_020029ac((s32)gOv20);
    Field_unk_02002a2c(120);
    Field_unk_02002b64(0x406218, 1);
    Field_unk_02002b6c(60);
    Field_unk_0200299c(60);
    gOv16 = 0;
    step_addr = (s32)&gOv17;
    gOv15 = 0x800000;
    *(s32 *)step_addr = 1;
    Field_unk_020029a4((s32)gOv21, 0xc80);
    Field_unk_02002a2c(180);
    Field_unk_02002bcc(21);
    Scene_RunPairedStepA(1, 80);
    Scene_RunPairedStepA(2, 40);
    Field_unk_02002b3c(0, 0x102);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002b3c(2, 0x102);
    Field_unk_02002b3c(3, 0x102);
    Field_unk_02002a2c(60);
    Scene_RunPairedStepA(2, 20);
    *(s32 *)step_addr = 2;
    Field_unk_02002ad4(2, 2);
    Field_unk_02002a2c(20);
    Field_unk_02002ad4(1, 1);
    Field_unk_02002a2c(40);
    Field_unk_02002ad4(0, 2);
    Field_unk_02002ad4(3, 1);
    Field_unk_02002a2c(20);
    Field_unk_02002ad4(2, 3);
    Field_unk_02002a2c(40);
    Field_unk_02002ad4(0, 1);
    Field_unk_02002a2c(20);
    Field_unk_02002ad4(1, 2);
    Field_unk_02002a2c(20);
    Field_unk_02002ad4(3, 2);
    Field_unk_02002b3c(1, 0x102);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_unk_02002b3c(3, 0x102);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    work_addr = (s32)&gOv17;
    *(s32 *)work_addr = 3;
    Field_unk_02002a54(0)->unknown_23 &= 254;
    Field_unk_02002a54(1)->unknown_23 &= 254;
    Field_unk_02002a54(2)->unknown_23 &= 254;
    Field_unk_02002a54(3)->unknown_23 &= 254;
    Field_unk_02002b2c(0, 3);
    Field_unk_02002b2c(1, 3);
    Field_unk_02002b2c(2, 3);
    value = 0;
    Field_unk_02002b2c(3, 3);
    gOv18 = value;
    Field_unk_020029a4((s32)gOv19, 0xc80);
    Field_unk_02002bcc(220);
    Field_unk_02002a54(19)->unknown_23 &= 254;
    Field_unk_02002b2c(19, 2);
    Field_unk_02002ab4(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_unk_02002a6c(19, action_c);
    Field_unk_02002a54(20)->unknown_23 &= 254;
    Field_unk_02002b2c(20, 2);
    Field_unk_02002ab4(20, 0x640000, 0x1120000);
    Field_unk_02002a6c(20, action_c);
    if (gOv14 != 0) {
        Field_unk_02002a54(21)->unknown_23 &= 254;
        Field_unk_02002b2c(21, 2);
        Field_unk_02002ab4(21, 0x4a0000, 0xfe0000);
        Field_unk_02002a6c(21, action_c);
    }
    Field_unk_02002a54(22)->unknown_23 &= 254;
    Field_unk_02002b2c(22, 2);
    Field_unk_02002ab4(22, 0x5e0000, 0xe10000);
    Field_unk_02002a6c(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Field_unk_0200299c(1);
        } while (gOv17 != 0);
    }
    Field_TestFlag12c(0x12c);
    Field_unk_020029ac((s32)gOv21);
    Field_unk_02002a2c(120);
    Field_unk_02002bcc(17);
    Field_unk_02002b64(0x10000, 1);
    Field_unk_02002b6c(60);
    Field_unk_0200299c(60);
    Field_unk_02002a7c(19);
    Field_unk_02002a7c(20);
    if (gOv14 != 0) {
        Field_unk_02002a7c(21);
    }
    (Field_unk_02002a7c)(22);
    Field_unk_0200299c(1);
    action_d = (s32)gOv28;
    (Field_unk_02002a6c)(19, action_d);
    Field_unk_02002a6c(20, action_d);
    if (gOv14 != 0) {
        Field_unk_02002a6c(21, action_d);
    }
    Field_unk_02002a84(22, action_d);
    Field_unk_02002a2c(80);
    Field_unk_02002adc(1, 2);
    Field_unk_02002a2c(40);
    Field_unk_02002b0c(1, 0);
    Field_unk_02002ab4(17, 0x570000, 0x8b0000);
    Field_unk_02002ab4(18, 0x570000, 0x8b0000);
    Field_unk_0200299c(1);
    if (Field_unk_02002a4c(17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002adc(0, 1);
    Field_unk_02002a2c(20);
    Field_unk_02002adc(2, 2);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Field_unk_02002adc(3, 2);
        Field_unk_02002a2c(10);
        Field_unk_02002b04(0x1488);
        Scene_RunPairedStepA(3, 40);
    }
    Field_unk_02002ad4(1, 1);
    Field_unk_02002b34(1, 0x101, 0);
    Field_unk_02002a2c(80);
    Field_unk_02002adc(2, 2);
    Field_unk_02002b04(0x1489);
    Scene_RunPairedStepA(2, 40);
    Field_unk_02002adc(1, 3);
    Field_unk_02002a2c(40);
    Field_unk_02002b2c(1, 2);
    Field_unk_02002a54(1)->unknown_23 |= 1;
    record = Field_unk_02002a54(1);
    Field_unk_02002a0c((s32)record, 1);
    Field_unk_02002acc(1, 6, 0);
    Field_unk_02002abc(1, 1);
    Scene_RunPairedStepB(1, 0x4000, 60);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002ad4(1, 2);
    (Scene_RunPairedStepA)(1, 10);
    Field_unk_02002adc(0, 3);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Field_unk_02002b34(1, 0x101, 0);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepB(1, 0x6000, 40);
    Scene_RunPairedStepB(1, 0x2000, 20);
    Scene_RunPairedStepB(1, 0x6000, 20);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_unk_02002acc(1, 2, 0);
    Field_unk_02002a2c(40);
    Field_unk_02002acc(1, 2, 0);
    Field_unk_02002a2c(10);
    Field_unk_02002acc(1, 4, 0);
    Field_unk_02002a2c(20);
    Scene_RunPairedStepA(1, 20);
    if (gOv14 != 0) {
        Field_unk_02002b34(3, 0x100, 0);
        Field_unk_02002a2c(60);
        Field_unk_02002adc(3, 2);
        Field_unk_02002a2c(80);
        Field_unk_02002b2c(3, 2);
        Field_unk_02002a54(3)->unknown_23 |= 1;
        record = Field_unk_02002a54(3);
        Field_unk_02002a0c((s32)record, 1);
        Field_unk_02002acc(3, 4, 0);
        Field_unk_02002aa4(3, -2, 0);
        Field_unk_02002abc(3, 1);
        Scene_RunPairedStepB(3, 0xe000, 60);
        Field_unk_02002adc(3, 2);
        Field_unk_02002a2c(20);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002acc(1, 2, 0);
    Scene_RunPairedStepB(1, 0x4000, 20);
    Field_unk_02002ac4(1, 3);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002ac4(1, 3);
    Field_unk_02002a2c(10);
    Field_unk_02002adc(2, 1);
    Field_unk_02002a2c(40);
    Field_unk_02002adc(2, 2);
    Field_unk_02002a2c(20);
    value = 1;
    Field_unk_02002b2c(2, 2);
    Field_unk_02002a54(2)->unknown_23 |= value;
    record = Field_unk_02002a54(2);
    Field_unk_02002a0c((s32)record, 1);
    Field_unk_02002acc(2, 4, 0);
    Field_unk_02002abc(2, 1);
    Field_unk_02002b24(2, 0xc000, 0);
    Field_unk_02002adc(0, 2);
    Field_unk_02002a2c(10);
    Field_unk_02002b2c(0, 2);
    Field_unk_02002a54(0)->unknown_23 |= value;
    record = Field_unk_02002a54(0);
    Field_unk_02002a0c((s32)record, 1);
    Field_unk_02002acc(0, 4, 0);
    Field_unk_02002abc(0, 1);
    Scene_RunPairedStepB(0, 0x6000, 60);
    Field_unk_02002b34(0, 0x105, 0);
    Field_unk_02002b34(2, 0x105, 0);
    Field_unk_02002a2c(60);
    Scene_RunPairedStepB(0, 0xa000, 20);
    Field_unk_02002ac4(1, 3);
    Field_unk_02002ac4(0, 3);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_unk_02002b24(2, 0xc000, 0);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(2, 20);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(3, 3);
    Field_unk_02002ac4(1, 3);
    Field_unk_02002a2c(20);
    Field_unk_02002adc(2, 1);
    Field_unk_02002a2c(20);
    Field_unk_02002b0c(2, 0);
    Field_unk_02002abc(2, 3);
    Field_unk_02002b24(2, 0xe000, 0);
    Field_unk_02002b24(1, 0x2000, 0);
    if (Field_unk_02002a4c(0, 0) == 0) {
        Field_unk_02002abc(2, 3);
        Field_unk_02002ac4(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Field_unk_02002adc(1, 2);
        Scene_RunPairedStepB(1, 0x2000, 10);
        Field_unk_02002ad4(1, 2);
        Field_unk_02002b14(1, 0);
    }
    Scene_RunPairedStepB(1, 0x4000, 10);
    Field_unk_02002ac4(1, 4);
    Scene_RunPairedStepA(1, 20);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(2, 10);
    if (gOv14 != 0) {
        Field_unk_02002adc(3, 2);
        Scene_RunPairedStepB(3, 0, 20);
        Scene_RunPairedStepB(3, 0x2000, 10);
        Field_unk_02002abc(3, 4);
        Scene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002b24(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 10);
    Field_unk_02002abc(0, 3);
    Field_unk_02002ac4(1, 3);
    value = 128;
    Field_unk_02002a2c(20);
    Field_unk_02002b24(0, 0x6000, 0);
    Scene_RunPairedStepB(1, (value << 7), 10);
    Field_unk_02002ac4(2, 4);
    Field_unk_02002a2c(20);
    Field_unk_02002b34(0, 0x102, 0);
    Field_unk_02002b34(1, 0x102, 80);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_unk_02002ad4(2, 2);
    Scene_RunPairedStepA(2, 20);
    Field_unk_02002b24(1, 0x2000, 0);
    Scene_RunPairedStepB(0, 0xa000, 40);
    Field_unk_02002b24(1, (value << 7), 0);
    Scene_RunPairedStepB(0, 0x6000, 10);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002ac4(2, 3);
    Field_unk_02002a2c(20);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002abc(2, 3);
    Scene_RunPairedStepA(2, 10);
    Field_unk_02002adc(1, 2);
    Scene_RunPairedStepB(1, 0x2000, 10);
    Field_unk_02002b0c(1, 0);
    Field_unk_02002b24(0, 0xa000, 0);
    if (Field_unk_02002a4c(0, 0) == 0) {
        Field_unk_02002ac4(1, 3);
    } else {
        Field_unk_02002a2c(20);
        Field_unk_02002adc(1, 2);
        Field_unk_02002a2c(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002b14(1, 0);
    Field_unk_02002bcc(21);
    Field_unk_02002b64(0x406218, 1);
    Field_unk_02002b6c(60);
    Field_unk_0200299c(60);
    gOv16 = 0;
    gOv15 = 0x800000;
    step_next = (s32)&gOv17;
    *(s32 *)step_next = 1;
    Field_unk_020029a4((s32)gOv21, 0xc80);
    Field_unk_02002a2c(80);
    Field_unk_02002ad4(0, 2);
    Field_unk_02002ad4(1, 2);
    Field_unk_02002ad4(3, 2);
    Field_unk_02002adc(2, 2);
    Field_unk_02002a2c(60);
    Scene_RunPairedStepB(2, 0xc000, 10);
    Field_unk_02002b04(0x149d);
    Scene_RunPairedStepA(2, 10);
    Scene_RunPairedStepB(1, 0xc000, 10);
    Scene_RunPairedStepB(0, 0xc000, 10);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0xc000, 10);
    }
    Field_unk_02002a54(0)->unknown_23 &= 254;
    Field_unk_02002a54(1)->unknown_23 &= 254;
    Field_unk_02002a54(2)->unknown_23 &= 254;
    Field_unk_02002a54(3)->unknown_23 &= 254;
    Field_unk_02002b2c(0, 3);
    Field_unk_02002b2c(1, 3);
    Field_unk_02002b2c(2, 3);
    Field_unk_02002b2c(3, 3);
    *(s32 *)step_next = 2;
    Field_unk_02002bcc(220);
    Field_unk_02002ab4(19, 0x780000, 0xf80000);
    action_c = (s32)gOv27;
    Field_unk_02002a6c(19, action_c);
    Field_unk_02002ab4(20, 0x640000, 0x1120000);
    Field_unk_02002a6c(20, action_c);
    if (gOv14 != 0) {
        Field_unk_02002ab4(21, 0x4a0000, 0xfe0000);
        Field_unk_02002a6c(21, action_c);
    }
    Field_unk_02002ab4(22, 0x5e0000, 0xe10000);
    Field_unk_02002a6c(22, action_c);
    Field_unk_02002a2c(120);
    *(s32 *)step_next = 3;
    do {
        Field_unk_0200299c(1);
    } while (gOv17 != 0);
    Scene_RunPairedStepA(17, 80);
    Scene_RunPairedStepA(18, 20);
    Field_unk_02002b34(0, 0x101, 0);
    Field_unk_02002b34(1, 0x101, 0);
    Field_unk_02002b34(2, 0x101, 0);
    Field_unk_02002b34(3, 0x101, 0);
    Field_unk_02002a2c(60);
    Scene_RunPairedStepA(18, 20);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(1, 3);
    Field_unk_02002abc(3, 3);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(1, 3);
    Field_unk_02002abc(3, 3);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002b34(0, 0x100, 0);
    Field_unk_02002b34(1, 0x100, 0);
    Field_unk_02002b34(2, 0x100, 0);
    Field_unk_02002b34(3, 0x100, 0);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepA(17, 10);
    Field_unk_02002b24(0, 0x8000, 0);
    Field_unk_02002b24(1, 0x4000, 0);
    Field_unk_02002b24(3, 0, 0);
    Scene_RunPairedStepB(2, 0xc000, 40);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002b3c(0, 0x102);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002b3c(2, 0x102);
    Field_unk_02002b3c(3, 0x102);
    Field_unk_02002b24(0, 0xc000, 0);
    Field_unk_02002b24(1, 0xc000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 80);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002b24(0, 0x8000, 0);
    Field_unk_02002b24(1, 0x4000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 40);
    Scene_RunPairedStepA(17, 10);
    Field_unk_02002b24(0, 0xc000, 0);
    Field_unk_02002b24(1, 0xc000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Field_unk_02002abc(0, 4);
    Field_unk_02002abc(1, 4);
    Field_unk_02002abc(3, 4);
    Field_unk_02002ac4(2, 4);
    Field_unk_02002a2c(60);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(1, 3);
    Field_unk_02002abc(3, 3);
    Field_unk_02002ac4(2, 3);
    Scene_RunPairedStepA(18, 20);
    Field_unk_02002b24(0, 0x8000, 0);
    Field_unk_02002b24(1, 0x4000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002ad4(0, 2);
    Field_unk_02002ad4(1, 2);
    Field_unk_02002ad4(3, 2);
    Field_unk_02002adc(2, 2);
    Field_unk_02002b24(0, 0xc000, 0);
    Field_unk_02002b24(1, 0xc000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Field_unk_02002b24(3, 0xc000, 0);
    Scene_RunPairedStepA(18, 20);
    Field_unk_02002b24(0, 0x8000, 0);
    Field_unk_02002b24(1, 0x4000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0, 20);
    Scene_RunPairedStepA(17, 20);
    Field_unk_02002b3c(0, 0x102);
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002b3c(3, 0x102);
    Field_unk_02002b3c(2, 0x102);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002b24(0, 0xc000, 0);
    Field_unk_02002b24(1, 0xc000, 0);
    Field_unk_02002b24(2, 0xc000, 0);
    Scene_RunPairedStepB(3, 0xc000, 10);
    Scene_RunPairedStepA(18, 10);
    Field_unk_02002abc(0, 3);
    Field_unk_02002abc(1, 3);
    Field_unk_02002abc(3, 3);
    Field_unk_02002ac4(2, 3);
    Field_unk_02002a2c(60);
    Field_unk_02002b14(18, 0);
    Field_unk_02002b14(17, 0);
    Field_unk_020029ac((s32)gOv21);
    Field_unk_02002a2c(80);
    Field_unk_02002b64(0x10000, 1);
    Field_unk_02002b6c(60);
    Field_unk_0200299c(80);
    Field_unk_02002a7c(19);
    Field_unk_02002a7c(20);
    work_addr = (s32)&gOv14;
    Field_unk_02002a7c(21);
    Field_unk_02002a7c(22);
    Field_unk_0200299c(1);
    action_d = (s32)gOv28;
    Field_unk_02002a6c(19, action_d);
    Field_unk_02002a6c(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Field_unk_02002a6c(21, action_d);
    }
    Field_unk_02002a84(22, action_d);
    Field_unk_02002a2c(20);
    Field_unk_02002b2c(0, 2);
    Field_unk_02002b2c(1, 2);
    Field_unk_02002b2c(2, 2);
    value = 1;
    Field_unk_02002b2c(3, 2);
    Field_unk_02002a54(0)->unknown_23 |= value;
    Field_unk_02002a54(1)->unknown_23 |= value;
    Field_unk_02002a54(2)->unknown_23 |= value;
    Field_unk_02002a54(3)->unknown_23 |= value;
    Field_unk_02002adc(2, 2);
    Scene_RunPairedStepB(2, 0xe000, 10);
    Field_unk_02002b0c(2, 0);
    Field_unk_02002b24(1, 0x2000, 0);
    Field_unk_02002b24(3, 0, 0);
    if (Field_unk_02002a4c(0, 0) == 0) {
        Field_unk_02002adc(1, 2);
        Field_unk_02002a2c(10);
        Field_unk_02002b0c(1, 0);
        if (Field_unk_02002a4c(0, 0) == 0) {
            Scene_RunPairedStepB(3, 0, 20);
            Field_unk_02002b34(1, 0x101, 0);
            Field_unk_02002b34(2, 0x101, 0);
            Field_unk_02002b34(3, 0x101, 0);
            Field_unk_02002a2c(40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Scene_RunPairedStepA(1, 10);
            Scene_RunPairedStepB(2, 0xc000, 20);
            Scene_RunPairedStepB(2, 0xe000, 20);
            Field_unk_02002ac4(2, 3);
            Scene_RunPairedStepA(2, 20);
            Scene_RunPairedStepB(1, 0x2000, 20);
        } else {
            Scene_RunPairedStepB(3, 0, 20);
            Field_unk_02002b34(1, 0x102, 0);
            Field_unk_02002b34(2, 0x102, 0);
            Field_unk_02002b34(3, 0x102, 40);
            Scene_RunPairedStepB(1, 0x4000, 20);
            Field_unk_02002b04(0x14b4);
            Scene_RunPairedStepA(1, 20);
            Field_unk_02002ac4(2, 3);
            Scene_RunPairedStepA(2, 20);
        }
        Field_unk_02002abc(3, 3);
        Field_unk_02002ac4(1, 3);
    } else {
        Field_unk_02002a2c(20);
        Field_unk_02002ac4(1, 3);
        Field_unk_02002a2c(10);
        Field_unk_02002b04(0x14b6);
        Scene_RunPairedStepA(1, 10);
        Field_unk_02002b24(1, 0x4000, 0);
        Scene_RunPairedStepB(0, 0x6000, 20);
        Field_unk_02002abc(1, 3);
        Field_unk_02002ac4(0, 3);
        Field_unk_02002a2c(10);
        Field_unk_02002ac4(2, 4);
        Field_unk_02002b0c(2, 0);
        if (Field_unk_02002a4c(0, 0) != 0) {
            goto L_0200239c;
        }
        Field_unk_02002a2c(20);
        Field_unk_02002b34(2, 0x103, 0);
        Field_unk_02002a2c(40);
        Scene_RunPairedStepB(2, 0xe000, 10);
        Scene_RunPairedStepA(2, 10);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0, 10);
            Field_unk_02002ad4(3, 3);
            Scene_RunPairedStepA(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_unk_02002b3c(0, 0x102);
        Field_unk_02002b3c(1, 0x102);
        Field_unk_02002a2c(40);
        Field_unk_02002adc(1, 2);
        Scene_RunPairedStepA(1, 20);
        Field_unk_02002b34(1, 0x105, 0);
        Field_unk_02002a2c(120);
        Scene_RunPairedStepA(2, 40);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(3, 0x2000, 10);
            Field_unk_02002ac4(3, 4);
            Scene_RunPairedStepA(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Field_unk_02002a2c(60);
        Field_unk_02002adc(2, 2);
        if (gOv14 != 0) {
            Scene_RunPairedStepB(2, 0xa000, 40);
            Scene_RunPairedStepB(2, 0xe000, 20);
        }
        Scene_RunPairedStepA(2, 10);
        Field_unk_02002ad4(0, 2);
        Field_unk_02002adc(1, 2);
        Field_unk_02002a2c(40);
        Scene_RunPairedStepA(2, 20);
        Field_unk_02002ac4(0, 3);
        Field_unk_02002ac4(1, 3);
        Field_unk_02002a2c(20);
        Field_unk_02002abc(3, 3);
    }
    Field_unk_02002ac4(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Field_unk_02002b34(2, 0x105, 0);
    Field_unk_02002a2c(40);
    Field_unk_02002ac4(2, 3);
    Field_unk_02002b04(0x14bf);
    Scene_RunPairedStepA(2, 20);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0, 10);
        Field_unk_02002ad4(3, 1);
        Scene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002b3c(1, 0x102);
    Field_unk_02002b3c(0, 0x102);
    Field_unk_02002a2c(40);
    Field_unk_02002adc(1, 2);
    Scene_RunPairedStepA(1, 20);
    Field_unk_02002b34(2, 0x105, 0);
    Field_unk_02002a2c(80);
    Scene_RunPairedStepA(2, 40);
    if (gOv14 != 0) {
        Scene_RunPairedStepB(3, 0x2000, 20);
        Field_unk_02002abc(3, 4);
        Scene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Field_unk_02002adc(2, 2);
    Field_unk_02002a2c(20);
    Scene_RunPairedStepA(2, 20);
    Field_unk_02002ad4(1, 2);
    Field_unk_02002adc(0, 2);
    Field_unk_02002a2c(40);
    Scene_RunPairedStepA(2, 20);
    L_020024a0:;
    Field_unk_02002bcc(17);
    Field_unk_02002a64(1, 0x13333, 0x9999);
    Field_unk_02002a64(2, 0x13333, 0x9999);
    Field_unk_02002a64(3, 0x13333, 0x9999);
    Field_unk_02002abc(1, 2);
    record = Pointer1(Field_unk_02002a54, 0);
    if ((s32)record != 0) {
        Field_unk_02002a8c(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_unk_02002aac(1);
    Field_unk_02002ab4(1, 0, 0);
    Field_unk_02002abc(2, 2);
    record = Pointer1(Field_unk_02002a54, 0);
    if ((s32)record != 0) {
        Field_unk_02002a8c(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Field_unk_02002aac(2);
    Field_unk_02002ab4(2, 0, 0);
    Field_unk_02002ab4(17, 0, 0);
    Field_unk_02002ab4(18, 0, 0);
    if (gOv14 != 0) {
        Field_unk_02002abc(3, 2);
        record = Pointer1(Field_unk_02002a54, 0);
        if ((s32)record != 0) {
            Field_unk_02002a8c(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Field_unk_02002aac(3);
        Field_unk_02002ab4(3, 0, 0);
    }
    Field_TestFlag843(0x843);
    Field_unk_02002bb4();
    Field_unk_02002a3c();
}
