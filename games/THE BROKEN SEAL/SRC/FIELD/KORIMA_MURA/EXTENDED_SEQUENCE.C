#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define SCENE_POINTER (*(u8 **)0x03001ebc)

#include "RESOURCE_38F.H"

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

s32 Func_02002a0a(struct Obj *);
u16 Func_02002af2(s32, s32);
void Func_0200029e(s32);
struct Rec *Func_02002c8a(s32);
void Func_02002c70(struct Rec *);
void Func_02002e3a(s32, s32, s32);
s32 Func_02002d10();
void Func_02002fbc(void);
s32 Func_0200311e();
s32 Func_0200312a();
s32 Func_02003136();
s32 Func_02003142();
s32 Func_0200317c();
void Func_02001078(void);
void Func_02003216_a();
struct Ent *Func_0200325a(s32);
struct Ent *Func_02003282(s32);
struct Ent *Func_020032aa(s32);
struct Ent *Func_020032d2(s32);
void Func_0200348a(void);
struct Obj *Func_02005006(s32, s32, s32, s32);
void Func_0200506c(struct Obj *, s32, s32, s32);

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

s32 SceneState_FlushPendingWordB698(s32 arg0)
{
    if (*(s32 *)0x0200B698 != 0) {
        Object_SetAnimation(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}

s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Func_02002a0a(p);
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Object_SetPalette(p, 7);
    } else {
        Object_SetPalette(p, 10);
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

s32 SceneEffect_UpdateFallingObject(struct Obj *p)
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
            Engine_ObjectDispatchRelease(p);
        }
    }
    return 1;
}

s32 SceneActor_TurnTowardTarget(struct Ent *p)
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
        if (GameFlag_IsSet(0x845) == 0) {
            Func_0200029e((s32)Data_0200b098);
        }
        return (s32)Data_0200b098;
    }
    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200b368;
    }
    return (s32)Data_0200b080;
}

void FieldScene_RunActor16MessageBranch(void)
{
    struct Rec *q = Func_02002c8a(0);
    s32 v = q->f06;
    Func_02002c70(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Sanctum_Open(16);
    } else {
        Event_SetMessage(0x16b3);
        Event_AskYesNo(16, 0);
    }
    Event_End();
}

void FieldScene_RunActor27Step(void)
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

void FieldScene_ConfigureActor0ThenRun(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Func_02002d10(0) + 85) = 0;
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -8);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 16;
    Event_RequestExit(a0);
}

s32 FieldScene_SetupEntryBySelector(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;
    s32 x;
    s32 y;

    step = Data_02000240[224];

    if (step == (s32)&Value_00000027) {
        Func_02002fbc();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Actor_SetSpriteFlags(Func_0200311e(23), 0);
    Actor_SetSpriteFlags(Func_0200312a(24), 0);
    Actor_SetSpriteFlags(Func_02003136(25), 0);
    Actor_SetSpriteFlags(Func_02003142(26), 0);

    tbl = Data_0200add8;
    Actor_EnableActionCallback(23, tbl);
    Actor_EnableActionCallback(24, tbl);
    Actor_EnableActionCallback(25, tbl);
    Actor_EnableActionCallback(26, tbl);

    if (GameFlag_IsSet(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Actor_SetSpriteFlags(Func_0200317c(actor), 0);
        }
        Map_CopyCellAttributes(13, 9, 1, 1, 13, 8);
        Map_CopyCellAttributes(13, 9, 1, 1, 15, 8);
        x = 14;
        y = 9;
        Map_CopyCellAttributes(13, 9, 1, 1, x, y);
    }

    if (GameFlag_IsSet(0x843) == 0) {
        if (Data_02000240[225] == 1) {
            Func_02001078();
        }
    }

    if (GameFlag_IsSet(0x843) != 0) {
        Actor_Destroy(1);
        Actor_Destroy(2);
        Actor_Destroy(3);
        Actor_Destroy(17);
        Actor_Destroy(18);
        Actor_Destroy(19);
        Actor_Destroy(20);
        Actor_Destroy(21);
        Actor_Destroy(22);
        Func_02003216_a(Data_0200b2d8);
    }

    return 0;
}

void SceneActor_SetActors19To22HeightByFrameParity(void)
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

void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1)
{
    Psynergy_Begin(141, 1);
    Psynergy_SetTarget(arg0, arg1);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void FieldScene_RunSequenceA(void)
{
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
    Func_0200348a();
}

#include "OBJECT_RUNTIME.H"


struct ObjectRuntime *Func_02002a54();
void Func_02002a74();
void Func_020025a8();
void Func_020008ac();
void Func_020008d8();
void Func_020025c0();
s32 Func_020029a4();
s32 Func_020029ac();
void Func_02002a84();
void Func_02002bb4();

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

void FieldScene_RunExtendedActorSequence(void)
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

    Event_Begin();
    flag_addr = (s32)&Data_0200b69c;
    *(s32 *)flag_addr = GameFlag_IsSet(3);
    record = Func_02002a54(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002a54(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002a54(21);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Func_02002a54(22);
    Actor_SetSpriteFlags((s32)record, 0);
    Camera_MoveTo(0x680000, -1, 0x1000000, 0);
    Map_Redraw();
    Actor_SetPosition(0, 0x170000, 0xf70000);
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 121, 238);
    Actor_SetSpeed(1, 0x9999, 0x4ccc);
    Actor_SetSpeed(2, 0x9999, 0x4ccc);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Actor_SetPosition(1, record->x, record->z);
    }
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Actor_SetPosition(2, record->x, record->z);
    }
    Actor_EnableActionCallback(1, (s32)Data_0200abd4);
    Actor_EnableActionCallback(2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Actor_SetSpeed(3, 0x9999, 0x4ccc);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Actor_SetPosition(3, record->x, record->z);
        }
        Actor_EnableActionCallback(3, (s32)Data_0200ac3c);
    }
    Func_02002a74(2);
    Actor_FaceDirection(0, 0xe000, 20);
    Actor_FaceDirection(2, 0xe000, 20);
    Actor_FaceDirection(1, 0x2000, 20);
    flag_addr = (s32)&Data_0200b69c;
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(3, 0x2000, 0);
    }
    Actor_FaceDirection(0, 0x2000, 20);
    Actor_FaceDirection(2, 0x2000, 20);
    Actor_FaceDirection(1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(3, 0xe000, 0);
    }
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceActor(2, 0, 0);
    Actor_FaceActor(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(3, 0, 0);
    }
    Event_Wait(40);
    Actor_StartRepeatedMotion(1, 1);
    Event_SetMessage(0x1473);
    Func_020025a8(1, 20);
    Actor_FaceActor(0, 1, 0);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(2, 72, 0x11e);
    Actor_WalkToAndWait(2, 72, 0x12e);
    Actor_WalkToAndWait(2, 88, 0x136);
    Actor_RunRepeatedMotion(2, 1);
    Actor_FaceActor(2, 0, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 2, 0);
    Actor_FaceActor(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(3, 2, 0);
    }
    Event_Wait(30);
    Actor_SetAnimation(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Actor_SetAnimation(3, 3);
    }
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Actor_FaceDirection(2, 0, 0);
    Event_Wait(20);
    Func_020008ac(2, 9);
    Event_Wait(40);
    Func_020008d8();
    Actor_ShowEmote(2, 0x100, 40);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Func_02002a54(2)->action_flags &= 254;
    Actor_WalkToAndWait(2, 80, 0x136);
    mask = 1;
    Event_Wait(1);
    Func_02002a54(2)->action_flags |= mask;
    Actor_ShowEmote(1, 0x102, 40);
    Func_020025a8(1, 20);
    Actor_ShowEmote(2, 0x102, 40);
    Func_020025a8(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(3, 0, 0);
    }
    Actor_FaceEachOther(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(3, 0xc000, 0);
    }
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_StartRepeatedMotion(1, 1);
    Func_020025a8(1, 20);
    Actor_SetSpeed(2, 0x8000, 0x4000);
    Func_02002a54(2)->action_flags &= 254;
    Actor_WalkToAndWait(2, 72, 0x11e);
    Event_Wait(1);
    Func_02002a54(2)->action_flags |= mask;
    Actor_EnableActionCallback(2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Actor_ShowEmote(3, 0x105, 0);
        Event_Wait(60);
        Func_020025a8(3, 20);
        Actor_SetAnimation(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_FaceActor(2, 0, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(10);
    Func_020025c0(1, 0x2000, 10);
    Func_020025c0(0, 0xa000, 10);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimation(1, 3);
    } else {
        Actor_SetAnimationAndWait(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020025a8(1, 40);
    Func_020025c0(2, 0x2000, 40);
    Func_020025c0(2, 0x8000, 20);
    Func_020025c0(2, 0x4000, 40);
    Actor_ShowEmote(2, 0x101, 0);
    Event_Wait(60);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 60);
    value = 160;
    Func_020025c0(3, 0x2000, 10);
    Actor_FaceDirection(1, 0x2000, 0);
    Func_020025c0(0, (value << 8), 10);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(0, 0x101, 0);
    Event_Wait(40);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 10);
    Actor_RunRepeatedMotion(1, 2);
    Event_SetMessage(0x147b);
    Func_020025a8(1, 10);
    Func_020025c0(2, 0xc000, 20);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(2, 20);
    Func_020025c0(1, 0, 20);
    Func_020025c0(0, (value << 8), 40);
    Func_020025c0(1, 0x4000, 20);
    Func_020025c0(0, 0x6000, 30);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(0, 0xe000, 30);
    Actor_RunRepeatedMotion(2, 2);
    Actor_ShowEmote(2, 0x100, 0);
    Event_Wait(40);
    Actor_FaceDirection(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 20);
    Func_020025c0(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    Data_0200b6a0 = 1;
    Value2(Func_020029a4, (s32)Data_0200a609, 0xc80);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(120);
    Task_Wait(60);
    action_a = (s32)Data_0200ac70;
    Actor_EnableActionCallback(0, action_a);
    Actor_EnableActionCallback(1, action_a);
    Actor_EnableActionCallback(2, action_a);
    Actor_EnableActionCallback(3, action_a);
    Event_Wait(100);
    Func_020025a8(1, 20);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Event_Wait(40);
        Actor_SetAttachedEffect(3, 0x102);
        Event_Wait(40);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_Wait(20);
    if (Data_0200b69c != 0) {
        value = 128;
        record = Func_02002a54(3);
        record->velocity_y = (value << 10);
        Event_Wait(10);
        Actor_SetSpeed(3, (value << 10), (value << 10));
        Actor_SetDestinationOffset(3, -2, 0);
        Actor_EnableActionCallback(3, (s32)Data_0200acfc);
        record = Func_02002a54(3);
        Actor_SetSpriteFlags((s32)record, 0);
        Actor_SetAnimation(3, 19);
        Event_Wait(10);
    }
    value = 128;
    record = Func_02002a54(0);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(0, (value << 10), (value << 10));
    action_b = (s32)Data_0200acfc;
    Actor_EnableActionCallback(0, action_b);
    record = Func_02002a54(0);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(0, 19);
    Event_Wait(20);
    record = Pointer1(Func_02002a54, 1);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(1, (value << 10), (value << 10));
    Actor_EnableActionCallback(1, action_b);
    record = Func_02002a54(1);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(1, 19);
    Event_Wait(40);
    record = Pointer1(Func_02002a54, 2);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_EnableActionCallback(2, action_b);
    record = Func_02002a54(2);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(2, 19);
    Data_0200b6a0 = 0;
    Event_Wait(160);
    Value1(Func_020029ac, (s32)Data_0200a609);
    Event_Wait(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Data_0200b690 = 0;
    step_addr = (s32)&Data_0200b694;
    Data_0200b68c = 0x800000;
    *(s32 *)step_addr = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Event_Wait(180);
    Audio_PlayCue(21);
    Func_020025a8(1, 80);
    Func_020025a8(2, 40);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(2, 0x102);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(60);
    Func_020025a8(2, 20);
    *(s32 *)step_addr = 2;
    Actor_StartRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(1, 1);
    Event_Wait(40);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(2, 3);
    Event_Wait(40);
    Actor_StartRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(3, 2);
    Actor_SetAttachedEffect(1, 0x102);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Actor_SetAttachedEffect(3, 0x102);
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
    Actor_SetSpritePriority(0, 3);
    Actor_SetSpritePriority(1, 3);
    Actor_SetSpritePriority(2, 3);
    value = 0;
    Actor_SetSpritePriority(3, 3);
    Data_0200b698 = value;
    Value2(Func_020029a4, (s32)Data_02008801, 0xc80);
    Audio_PlayCue(220);
    Func_02002a54(19)->unknown_23 &= 254;
    Actor_SetSpritePriority(19, 2);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Actor_EnableActionCallback(19, action_c);
    Func_02002a54(20)->unknown_23 &= 254;
    Actor_SetSpritePriority(20, 2);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Actor_EnableActionCallback(20, action_c);
    if (Data_0200b69c != 0) {
        Func_02002a54(21)->unknown_23 &= 254;
        Actor_SetSpritePriority(21, 2);
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Actor_EnableActionCallback(21, action_c);
    }
    Func_02002a54(22)->unknown_23 &= 254;
    Actor_SetSpritePriority(22, 2);
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Actor_EnableActionCallback(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Task_Wait(1);
        } while (Data_0200b694 != 0);
    }
    Event_Wait(0x12c);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Event_Wait(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Actor_Stop(19);
    Actor_Stop(20);
    if (Data_0200b69c != 0) {
        Actor_Stop(21);
    }
    (Engine_ActorStop)(22);
    Task_Wait(1);
    action_d = (s32)Data_0200ad7c;
    (Engine_ActorEnableActionCallback)(19, action_d);
    Actor_EnableActionCallback(20, action_d);
    if (Data_0200b69c != 0) {
        Actor_EnableActionCallback(21, action_d);
    }
    Func_02002a84(22, action_d);
    Event_Wait(80);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Event_OpenMessage(1, 0);
    Actor_SetPosition(17, 0x570000, 0x8b0000);
    Actor_SetPosition(18, 0x570000, 0x8b0000);
    Task_Wait(1);
    if (Event_ChooseYesNo(17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 2);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(10);
        Event_SetMessage(0x1488);
        Func_020025a8(3, 40);
    }
    Actor_StartRepeatedMotion(1, 1);
    Actor_ShowEmote(1, 0x101, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(2, 2);
    Event_SetMessage(0x1489);
    Func_020025a8(2, 40);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(40);
    Actor_SetSpritePriority(1, 2);
    Func_02002a54(1)->unknown_23 |= 1;
    record = Func_02002a54(1);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(1, 6, 0);
    Actor_SetAnimation(1, 1);
    Func_020025c0(1, 0x4000, 60);
    Func_020025a8(1, 20);
    Actor_StartRepeatedMotion(1, 2);
    (Func_020025a8)(1, 10);
    Actor_RunRepeatedMotion(0, 3);
    Func_020025c0(1, 0x2000, 20);
    Actor_ShowEmote(1, 0x101, 0);
    Event_Wait(40);
    Func_020025c0(1, 0x6000, 40);
    Func_020025c0(1, 0x2000, 20);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(1, 0x2000, 10);
    Actor_Jump(1, 2, 0);
    Event_Wait(40);
    Actor_Jump(1, 2, 0);
    Event_Wait(10);
    Actor_Jump(1, 4, 0);
    Event_Wait(20);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Actor_ShowEmote(3, 0x100, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(80);
        Actor_SetSpritePriority(3, 2);
        Func_02002a54(3)->unknown_23 |= 1;
        record = Func_02002a54(3);
        Actor_SetSpriteFlags((s32)record, 1);
        Actor_Jump(3, 4, 0);
        Actor_SetDestinationOffset(3, -2, 0);
        Actor_SetAnimation(3, 1);
        Func_020025c0(3, 0xe000, 60);
        Actor_RunRepeatedMotion(3, 2);
        Event_Wait(20);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_Jump(1, 2, 0);
    Func_020025c0(1, 0x4000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Func_020025c0(1, 0x2000, 10);
    Func_020025a8(1, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    value = 1;
    Actor_SetSpritePriority(2, 2);
    Func_02002a54(2)->unknown_23 |= value;
    record = Func_02002a54(2);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(2, 4, 0);
    Actor_SetAnimation(2, 1);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(10);
    Actor_SetSpritePriority(0, 2);
    Func_02002a54(0)->unknown_23 |= value;
    record = Func_02002a54(0);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(0, 4, 0);
    Actor_SetAnimation(0, 1);
    Func_020025c0(0, 0x6000, 60);
    Actor_ShowEmote(0, 0x105, 0);
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(60);
    Func_020025c0(0, 0xa000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetAnimationAndWait(0, 3);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(1, 0x4000, 10);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(2, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(20);
    Event_OpenMessage(2, 0);
    Actor_SetAnimation(2, 3);
    Actor_FaceDirection(2, 0xe000, 0);
    Actor_FaceDirection(1, 0x2000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(2, 3);
        Actor_SetAnimationAndWait(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Actor_RunRepeatedMotion(1, 2);
        Func_020025c0(1, 0x2000, 10);
        Actor_StartRepeatedMotion(1, 2);
        Event_ShowMessage(1, 0);
    }
    Func_020025c0(1, 0x4000, 10);
    Actor_SetAnimationAndWait(1, 4);
    Func_020025a8(1, 20);
    Func_020025c0(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(2, 10);
    if (Data_0200b69c != 0) {
        Actor_RunRepeatedMotion(3, 2);
        Func_020025c0(3, 0, 20);
        Func_020025c0(3, 0x2000, 10);
        Actor_SetAnimation(3, 4);
        Func_020025a8(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_FaceDirection(1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    value = 128;
    Event_Wait(20);
    Actor_FaceDirection(0, 0x6000, 0);
    Func_020025c0(1, (value << 7), 10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 80);
    Func_020025c0(2, 0xe000, 10);
    Actor_StartRepeatedMotion(2, 2);
    Func_020025a8(2, 20);
    Actor_FaceDirection(1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 40);
    Actor_FaceDirection(1, (value << 7), 0);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(2, 0xc000, 10);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(2, 10);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(40);
    Func_020025a8(1, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(40);
    Func_020025a8(1, 20);
    Actor_SetAnimation(2, 3);
    Func_020025a8(2, 10);
    Actor_RunRepeatedMotion(1, 2);
    Func_020025c0(1, 0x2000, 10);
    Event_OpenMessage(1, 0);
    Actor_FaceDirection(0, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(1, 3);
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_ShowMessage(1, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Data_0200b690 = 0;
    Data_0200b68c = 0x800000;
    step_next = (s32)&Data_0200b694;
    *(s32 *)step_next = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Event_Wait(80);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(60);
    Func_020025c0(2, 0xc000, 10);
    Event_SetMessage(0x149d);
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
    Actor_SetSpritePriority(0, 3);
    Actor_SetSpritePriority(1, 3);
    Actor_SetSpritePriority(2, 3);
    Actor_SetSpritePriority(3, 3);
    *(s32 *)step_next = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Actor_EnableActionCallback(19, action_c);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Actor_EnableActionCallback(20, action_c);
    if (Data_0200b69c != 0) {
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Actor_EnableActionCallback(21, action_c);
    }
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Actor_EnableActionCallback(22, action_c);
    Event_Wait(120);
    *(s32 *)step_next = 3;
    do {
        Task_Wait(1);
    } while (Data_0200b694 != 0);
    Func_020025a8(17, 80);
    Func_020025a8(18, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 0);
    Event_Wait(60);
    Func_020025a8(18, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(18, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(18, 10);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Event_Wait(40);
    Func_020025a8(17, 10);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(3, 0, 0);
    Func_020025c0(2, 0xc000, 40);
    Func_020025a8(18, 10);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(2, 0x102);
    Actor_SetAttachedEffect(3, 0x102);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 80);
    Func_020025a8(18, 10);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0, 40);
    Func_020025a8(17, 10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Actor_SetAnimation(0, 4);
    Actor_SetAnimation(1, 4);
    Actor_SetAnimation(3, 4);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(60);
    Func_020025a8(18, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Func_020025a8(18, 20);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(18, 10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Func_020025a8(18, 20);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(17, 20);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(3, 0x102);
    Actor_SetAttachedEffect(2, 0x102);
    Event_Wait(40);
    Func_020025a8(18, 10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Func_020025a8(18, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(60);
    Event_ShowMessage(18, 0);
    Event_ShowMessage(17, 0);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Event_Wait(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(80);
    Actor_Stop(19);
    Actor_Stop(20);
    work_addr = (s32)&Data_0200b69c;
    Actor_Stop(21);
    Actor_Stop(22);
    Task_Wait(1);
    action_d = (s32)Data_0200ad7c;
    Actor_EnableActionCallback(19, action_d);
    Actor_EnableActionCallback(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Actor_EnableActionCallback(21, action_d);
    }
    Func_02002a84(22, action_d);
    Event_Wait(20);
    Actor_SetSpritePriority(0, 2);
    Actor_SetSpritePriority(1, 2);
    Actor_SetSpritePriority(2, 2);
    value = 1;
    Actor_SetSpritePriority(3, 2);
    Func_02002a54(0)->unknown_23 |= value;
    Func_02002a54(1)->unknown_23 |= value;
    Func_02002a54(2)->unknown_23 |= value;
    Func_02002a54(3)->unknown_23 |= value;
    Actor_RunRepeatedMotion(2, 2);
    Func_020025c0(2, 0xe000, 10);
    Event_OpenMessage(2, 0);
    Actor_FaceDirection(1, 0x2000, 0);
    Actor_FaceDirection(3, 0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(10);
        Event_OpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Func_020025c0(3, 0, 20);
            Actor_ShowEmote(1, 0x101, 0);
            Actor_ShowEmote(2, 0x101, 0);
            Actor_ShowEmote(3, 0x101, 0);
            Event_Wait(40);
            Func_020025c0(1, 0x4000, 20);
            Func_020025a8(1, 10);
            Func_020025c0(2, 0xc000, 20);
            Func_020025c0(2, 0xe000, 20);
            Actor_SetAnimationAndWait(2, 3);
            Func_020025a8(2, 20);
            Func_020025c0(1, 0x2000, 20);
        } else {
            Func_020025c0(3, 0, 20);
            Actor_ShowEmote(1, 0x102, 0);
            Actor_ShowEmote(2, 0x102, 0);
            Actor_ShowEmote(3, 0x102, 40);
            Func_020025c0(1, 0x4000, 20);
            Event_SetMessage(0x14b4);
            Func_020025a8(1, 20);
            Actor_SetAnimationAndWait(2, 3);
            Func_020025a8(2, 20);
        }
        Actor_SetAnimation(3, 3);
        Actor_SetAnimationAndWait(1, 3);
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        Event_SetMessage(0x14b6);
        Func_020025a8(1, 10);
        Actor_FaceDirection(1, 0x4000, 0);
        Func_020025c0(0, 0x6000, 20);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(10);
        Actor_SetAnimationAndWait(2, 4);
        Event_OpenMessage(2, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
            goto L_0200239c;
        }
        Event_Wait(20);
        Actor_ShowEmote(2, 0x103, 0);
        Event_Wait(40);
        Func_020025c0(2, 0xe000, 10);
        Func_020025a8(2, 10);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0, 10);
            Actor_StartRepeatedMotion(3, 3);
            Func_020025a8(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Actor_SetAttachedEffect(0, 0x102);
        Actor_SetAttachedEffect(1, 0x102);
        Event_Wait(40);
        Actor_RunRepeatedMotion(1, 2);
        Func_020025a8(1, 20);
        Actor_ShowEmote(1, 0x105, 0);
        Event_Wait(120);
        Func_020025a8(2, 40);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0x2000, 10);
            Actor_SetAnimationAndWait(3, 4);
            Func_020025a8(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Event_Wait(60);
        Actor_RunRepeatedMotion(2, 2);
        if (Data_0200b69c != 0) {
            Func_020025c0(2, 0xa000, 40);
            Func_020025c0(2, 0xe000, 20);
        }
        Func_020025a8(2, 10);
        Actor_StartRepeatedMotion(0, 2);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(40);
        Func_020025a8(2, 20);
        Actor_SetAnimationAndWait(0, 3);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Actor_SetAnimation(3, 3);
    }
    Actor_SetAnimationAndWait(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(2, 3);
    Event_SetMessage(0x14bf);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0, 10);
        Actor_StartRepeatedMotion(3, 1);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(0, 0x102);
    Event_Wait(40);
    Actor_RunRepeatedMotion(1, 2);
    Func_020025a8(1, 20);
    Actor_ShowEmote(2, 0x105, 0);
    Event_Wait(80);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0x2000, 20);
        Actor_SetAnimation(3, 4);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Func_020025a8(2, 20);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(40);
    Func_020025a8(2, 20);
    L_020024a0:;
    Audio_PlayCue(17);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Actor_SetDestination(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Actor_SetDestination(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    if (Data_0200b69c != 0) {
        Actor_SetAnimation(3, 2);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Actor_SetDestination(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Actor_WaitForMove(3);
        Actor_SetPosition(3, 0, 0);
    }
    GameFlag_Set(0x843);
    Func_02002bb4();
    Event_End();
}

void FieldScene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Event_ShowMessage(arg0, 0);
    Event_Wait(arg1);
}

void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Actor_FaceDirection(arg0, arg1, 0);
    Event_Wait(arg2);
}

s32 SceneEffect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Engine_ObjectDispatchRelease(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
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
        Audio_PlayCue(200);
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
    Object_SetAnimation(p, 2);
    Func_0200506c(p, c3, 0, c4);
    Object_SetScript(p, Data_0200b5d8);
}

s32 SceneEffect_SetModeByFrameBit1(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Object_SetPalette(arg0, 10);
    } else {
        Object_SetPalette(arg0, 7);
    }
    return 0;
}

void FieldScene_RunFlag845And847Branches(void)
{
    if (GameFlag_IsSet(0x845) == 0) {
        Actor_SetPosition(8, 0, 0);
        { s32 k5 = 9, k6 = 18; Map_CopyCellAttributes(9, 17, 5, 1, k5, k6); }
        Map_Redraw();
        Task_Wait(1);
    } else {
        Actor_SetPosition(9, 0, 0);
    }
    if (GameFlag_IsSet(0x847) != 0) {
        { s32 k5 = 5, k6 = 7; Map_CopyCellsTo(91, 19, 72, 9, k5, k6); }
        { s32 k5 = 8, k6 = 11; Map_CopyCellAttributes(23, 11, 5, 7, k5, k6); }
        Map_Redraw();
        Task_Wait(1);
    }
}
