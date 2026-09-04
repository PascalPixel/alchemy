#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define FieldScene_ConfigureActor0ThenRun Func_020002b4
#define SCENE_POINTER (*(u8 *volatile *)0x03001ebc)

#include "resource_38f.h"

#define FieldScene_RunFlag845And847Branches Func_02002910

#define SceneEffect_SetModeByFrameBit1 Func_020026a4

#define SceneEffect_SpawnObject26EveryEightFrames Func_02002608

#define SceneEffect_AdvanceAngleUntilIdle Func_020025d8

#define FieldScene_RunPairedStepB Func_020025c0

#define FieldScene_RunPairedStepA Func_020025a8

#define FieldScene_RunSequenceA Func_020008d8

#define FieldScene_StartEffect141Sequence Func_020008ac

#define SceneActor_SetActors19To22HeightByFrameParity Func_02000800

#define FieldScene_SetupEntryBySelector Func_02000694

#define SceneData_SelectTableB3b0BySelector Func_02000284

#define FieldScene_RunActor27Step Func_02000274

#define FieldScene_RunActor16MessageBranch Func_02000230

#define SceneData_SelectTableB080BySelector Func_020001dc

#define SceneData_GetTableB040 Func_020001d4

#define SceneData_GetTableB010ForSelector26 Func_020001ac

#define SceneData_SelectTableAe60BySelector Func_0200016c

#define SceneActor_TurnTowardTarget Func_02000114

#define SceneEffect_UpdateFallingObject Func_020000cc

#define SceneData_InitRecordTable Func_020000a0

#define SceneEffect_AdvanceCounterAndSwitchMode Func_02000050

#define SceneState_FlushPendingWordB698 Func_02000030

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

/* Contiguous unnamed leaf-owner run for resource_38f. */

/*
 * resource_38f owner at 0x020001d4, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20001d8 holding 0x200b040.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020001d4 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200b040 is image offset
 * 0x3040 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 SceneState_FlushPendingWordB698(s32 arg0) {
    if (*(s32 *)0x0200B698 != 0) {
        Func_02002a02(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}

s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p) {
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

void SceneData_InitRecordTable(u8 *o) {
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

s32 SceneEffect_UpdateFallingObject(struct Obj *p) {
    p->f08 += p->f24;
    p->f10 += p->f2c;
    p->f2c += 0xf888f5c4;
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

s32 SceneActor_TurnTowardTarget(struct Ent *p) {
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
            if (t < (s32)0xf8b4f001) {
                t = (s32)0xf8b4f001;
            }
            p->f06 = p->f06 + t;
        }
    }
    return 1;
}

s32 SceneData_SelectTableAe60BySelector(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200af80;
    }
    if (v == (s32)&Value_00000026) {
        return (s32)Data_0200afc8;
    }
    return (s32)Data_0200ae60;
}

s32 SceneData_GetTableB010ForSelector26(void) {
    if (Data_02000240[224] == (s32)&Value_00000026) {
        return (s32)Data_0200b010;
    }
    return 0;
}

u8 *SceneData_GetTableB040(void)
{
    return (u8 *)0x0200b040;
}

s32 SceneData_SelectTableB080BySelector(void) {
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

void FieldScene_RunActor16MessageBranch(void) {
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

void FieldScene_RunActor27Step(void) {
    Func_02002e3a(27, 0, 1);
}

s32 SceneData_SelectTableB3b0BySelector(void) {
    if (Data_02000240[224] == (s32)&Value_00000027) {
        return (s32)Data_0200b590;
    }
    return (s32)Data_0200b3b0;
}

void FieldScene_ConfigureActor0ThenRun(s32 a0)
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

s32 FieldScene_SetupEntryBySelector(void)
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
            Func_02003172(13, 9, 1, 1, m, n);
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
        Func_02003216(Data_0200b2d8);
    }

    return 0;
}

void SceneActor_SetActors19To22HeightByFrameParity(void) {
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

void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1) {
    Func_02003444(141, 1);
    Func_02003454(arg0, arg1);
    Func_02003470();
    Func_0200344e(1);
    Func_0200326c(1);
}

void FieldScene_RunSequenceA(void) {
    Func_02003462(2);
    Func_0200347e();
    Func_0200348a();
}

void FieldScene_RunPairedStepA(s32 arg0, s32 arg1) {
    Func_020050c4(arg0, 0);
    Func_02004fe2(arg1);
}

void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2) {
    Func_020050ec(arg0, arg1, 0);
    Func_02004ffa(arg2);
}

s32 SceneEffect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p) {
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Func_02004fd6(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void) {
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

s32 SceneEffect_SetModeByFrameBit1(s32 arg0) {
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_020051ac(arg0, 10);
    } else {
        Func_020051b4(arg0, 7);
    }
    return 0;
}

void FieldScene_RunFlag845And847Branches(void) {
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
