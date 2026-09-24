#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SOUND_IDS.H"

#define CreateOverlayObject Func_02005d1e
#define SetOverlayObjectMode Func_02005d88
#define SetOverlayObjectSlot Func_02005ed0
#define OverlayObject_SetRecordField1 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_StepScaleByCounter Func_02000314
#define OverlayObject_DecayRecordField1e Func_02000400
#define OverlayObject_UpdateHeadingTimer Func_02000424
#define NULL ((void *)0)
#define OverlayObject_CreateAndInitialize Func_02000048
#define SceneEffect_UpdateObjectByFrameParity Func_020020dc
#define SceneEffect_AdvanceGatedRiseCounter Func_020059f0
#define SceneData_GetTablee3d4 Func_02000464
#define SceneData_ReturnZero Func_0200046c
#define SceneData_GetTablee464 Func_02000470
#define SceneData_GetTablee478 Func_02000478
#define SceneData_GetTableE6ec Func_02000714
#define MeasureFixedPointPositionDistance Func_02000480
#define SceneActor_FindNearestSlotOfKindF2 Func_020004bc
#define InitializeActorZeroMotion Func_02000518
#define SceneActor_ParkRecord Func_02005688
#define SceneEffect_SpawnAndBobWithActorZero Func_020005ec
#define SceneActor_SetByte55ForActorZeroAnd12To17 Func_020006c0
#define FieldScene_InitActorsAndDispatchBySubstate Func_0200071c
#define FieldScene_RunSetupSequence35c4 Func_020035c4
#define FieldScene_RunThreeStepsInBracket Func_020038c0
#define FieldScene_RunBracketedSceneWithFlag282 Func_020038dc
#define SceneState_ApplyArgMode0AndSet10 Func_02000894
#define SceneState_ForwardByRuntimeWordBits Func_0200211c
#define SceneState_ApplyPair140And0 Func_02002334
#define FieldScene_CallPairWith10 Func_020008a8
#define FieldScene_ForwardValue81fc Func_02002344
#define FieldScene_RunScene3c9_02001280 Func_02001280
#define FieldScene_RunStep6 Func_02002350
#define FieldScene_RunScene3c9_02003924 Func_02003924
#define FieldScene_RunScene3c9_02004b28 Func_02004b28
#define FieldScene_RunScene3c9_02005b90 Func_02005b90
#define Effect_AnimateVerticalPositive Func_0200215c
#define Effect_AnimateVerticalNegative Func_020021ac
#define SceneEffect_UpdateCounterDrivenOrbit Func_02003660
#define FieldScene_RunMultiActorPresentation Func_02003e9c

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

typedef struct {
    u8 filler0[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} Sub;

typedef struct {
    u8 filler0[0x50];
    Sub *sub;
} Obj;

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj_02000400;

typedef struct {
    u8 filler0[0x50];
    Obj_02000400 *obj;
} Spr_02000400;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr_02000424;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj_020004bc;

typedef struct {
    u8 filler0[0x50];
    Obj_020004bc *obj;
} Spr_020004bc;

struct SceneActor {
    u8 pad00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[20];
    s32 motion28;
    u8 pad2c[4];
    s32 motion30;
    s32 motion34;
    u8 pad38[29];
    u8 flags55;
};

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[0x3d];
    u8 unk55;
} Spr_020005ec;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr_0200071c;

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern unsigned char Data_0200e3d4[];
extern unsigned char Data_0200e464[];
extern unsigned char Data_0200e478[];
extern unsigned char Data_0200e6ec[];
extern Spr *Data_0200e6e8;

s32 Func_02005f8e(Spr *);
s32 Func_02005f74(s32, s32);
void OverlayObject_SetValue1(void *, s32);
s32 Func_02007cf8(s32, s32);
void Func_0200430e(s32);
void Func_0200b096(void *record);
typedef s32(*IwramIntegerSquareRoot)(s32);
void Func_02000c3a();
void Func_0200618e();
s32 Func_0200620e();
Spr *Func_02000ab8(void);
u8 *Func_02006412(s32);
u8 *Func_0200641e(s32);
u8 *Func_0200642a(s32);
u8 *Func_02006434(s32);
u8 *Func_0200643e(s32);
u8 *Func_02006448(s32);
u8 *Func_02006452(s32);
Spr *Func_020064b4(s32);
Spr *Func_020064be(s32);
void Func_020010a8(void);
void Func_02002b5a(void);
void Func_020040c0(void);
void Func_020040e2(void);
void Func_02004130(void);
void Func_02006534(s32, s32);
void Func_0200653e(s32, s32);
s32 Func_02000d1a(void);
Spr *Func_020065b2(void);
void Func_02007768();
void Func_02007b0c();
s32 Func_02007d28(u32, s32);
s32 Func_02007d38(u32, s32);
void Func_0200434e(s32);
void Func_020081fc(s32);
void Func_02004476(void);
void Func_02008784();
void Func_020090a2();
void Func_020090d4();
void Func_02009106();
void Func_02009134();
void Func_0200917e();
void Func_0200918e();
void Func_020091a2();
void Func_020091b2();
s32 Func_02009788();
s32 Func_02009898();
void Func_020053c8();
void Func_0200542c();
void Func_02005464();
void Func_0200478e();
void Func_020047be();
void Func_020047fa();
void Func_02004808();
void Func_02004834();
void Func_02004842();
void Func_0200484e();
void Func_02004874();
void Func_02004888();
void Func_020048da();
void Func_020048fe();
void Func_02004902();
void Func_02004926();
void Func_0200497c();
void Func_02004990();
void Func_0200499e();
void Func_020049b0();
void Func_020049d0();
void Func_020049e6();
void Func_020049fe();
void Func_02004a04();
void Func_02004a16();
void Func_02004a24();
void Func_02004a34();
void Func_02004a42();
void Func_02004a5a();
void Func_02004a68();
void Func_02004834_a();

/*
 * resource_3c9: a scripted-scene bracket around three setup steps.
 */

/* The three inner steps are old-style: their signatures are not established. */

/*
 * One symbol per call site, spelled as the raw pc-relative decode of the
 * reference halfwords rather than the semantic import address. Listed in
 * call order.
 */

/*
 * These three symbols name the call words each site holds, not the imports
 * they eventually reach. Respelling them as the semantic imports changes the
 * call encodings, so leave the addresses as they are.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call3_02003924(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3_02004b28(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 */
static __inline__ void Call2_02003e9c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_02003e9c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02008410();  /* Func_02004b28, this overlay's own */

void OverlayObject_SetRecordField1(Obj *o, u32 v)
{
    o->sub->f1 = v;
}

/* The +98 byte is read directly in both tests and incremented in place rather
 * than cached in a local; that shape is what the reference holds. */

/*
 * Gated by the byte flag at +99: recompute a fixed-point offset from the byte
 * counter at +98 into the u32 field at +12, call a leaf helper, then advance
 * that counter while it stays in (0, 31]. The 56-byte owner has no literal
 * pool. obj is read before it is overwritten, so it is a real argument. The
 * layout behind +12, +76, +98 and +99 is not established.
 */
void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Object_Create(kind, x, y, z);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        SetOverlayObjectMode(ret, 0);
        SetOverlayObjectSlot(ret, 14);
        OverlayObject_SetValue1(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

s32 OverlayObject_StepScaleByCounter(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = Func_02005f74(Func_02005f8e(s), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

/*
 * Moves an effect like Effect_Move, and slows its horizontal velocity by a
 * twenty-second across and a twentieth in depth each frame.
 */
void Effect_MoveWithDrag(union FieldObject *object)
{
    s32 velocity_x = object->effect.velocity_x;
    s32 velocity_z;

    object->effect.x += velocity_x;
    object->effect.y += object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.z += velocity_z;
    object->effect.velocity_x = velocity_x - Math_Divide(velocity_x, 22);
    object->effect.velocity_z = velocity_z - Math_Divide(velocity_z, 20);
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

void OverlayObject_DecayRecordField1e(Spr_02000400 *s)
{
    Obj_02000400 *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OverlayObject_UpdateHeadingTimer(Spr_02000424 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Random_Next() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Random_Next() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3c9. */
s32 SceneData_GetTablee3d4(void)
{
    return (s32)Data_0200e3d4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)Data_0200e464;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)Data_0200e478;
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Spr_020004bc **p;
    s32 best = 0;
    Spr_020004bc *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Actor_Get(0);
    i = 8;
    p = (Spr_020004bc **)(work + 0x34);
    do {
        Spr_020004bc *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = MeasureFixedPointPositionDistance((u8 *)ref + 8, (u8 *)spr + 8);
                if (dist < limit) {
                    limit = dist;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}

void InitializeActorZeroMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Actor_Get(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Func_0200618e(0x200000, angle, position);
    if (Func_0200620e(actor, position) == 0) {
        GameFlag_Clear(592);
        Func_02000c3a();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Object_SetAnimation(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Audio_PlayCue(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Actor_SetSpriteFlags(actor, 0);
        Actor_MoveToAndWait(0, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        actor->flags55 = flags;
    }
}

void SceneEffect_SpawnAndBobWithActorZero(void)
{
    Spr_020005ec *a = Actor_Get(0);
    Spr_020005ec *b;
    Spr_020005ec *r;
    s32 k;

    Event_Begin();
    r = Func_02000ab8();
    Data_0200e6e8 = r;
    if (r != 0) {
        GameFlag_Set(592);
        b = Actor_Get(Data_0200e6e8);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Task_Wait(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Task_Wait(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Event_End();
}

void SceneActor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *(Func_02006412(0) + 0x55) = 3;
    val = 4;
    *(Func_0200641e(12) + 0x55) = val;
    *(Func_0200642a(13) + 0x55) = val;
    *(Func_02006434(14) + 0x55) = val;
    *(Func_0200643e(15) + 0x55) = val;
    *(Func_02006448(16) + 0x55) = val;
    *(Func_02006452(17) + 0x55) = val;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)Data_0200e6ec;
}

s32 FieldScene_InitActorsAndDispatchBySubstate(void)
{
    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr_0200071c *obj;

    GameFlag_Set(324);
    Task_Wait(1);
    GameFlag_Set(272);
    Actor_SetSpriteFlags(Actor_Get(8), 0);
    Actor_SetSpriteFlags(Actor_Get(9), 0);
    Actor_SetSpriteFlags(Actor_Get(10), 0);
    Actor_SetSpriteFlags(Actor_Get(11), 0);
    v = (s32)0xffff0000;
    Func_020064b4(10)->unk18 = v;
    Func_020064be(11)->unk18 = v;
    i = 12;
    z = 0;
    do {
        obj = Actor_Get(i);
        Actor_SetSpriteFlags(Actor_Get(i), z);
        Actor_SetSpritePriority(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gGameState.entrance;

    switch (mode) {
    case 1:
        if (GameFlag_IsSet(0x109) == 0) {
            Func_020010a8();
        }
        break;
    case 2:
        Func_02002b5a();
        break;
    case 3:
        Func_020040c0();
        break;
    case 0x5d:
        Func_020040e2();
        break;
    case 4:
        Func_02004130();
        break;
    case 9:
        Event_Begin();
        if (GameFlag_IsSet(0x345) != 0) {
            Func_02006534(0, 65);
        } else if (GameFlag_IsSet(0x346) != 0) {
            Func_02006534(1, 65);
        } else if (GameFlag_IsSet(0x347) != 0) {
            Func_02006534(2, 65);
        } else {
            Func_0200653e(3, 65);
        }
        Event_RequestExit(9);
        break;
    }

    if (GameFlag_IsSet(0x109) != 0) {
        if (Func_02000d1a() != 0) {
            Spr_0200071c *p = Func_020065b2();
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void SceneState_ApplyArgMode0AndSet10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Actor_FaceDirection(a, b, 10);
}

void FieldScene_RunScene3c9_02001280(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    if (a1 != 0) {
        Actor_SetChildValue(a0, 0);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 1);
        Actor_SetSpeed(a0, 0xcccc, 0x6666);
    } else {
        Actor_SetChildValue(a0, 15);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 0);
    }
}

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)0x03001e40 & 2) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if (Func_02007cf8(*(s32 *)0x03001e40, 15) == 0) {
        Func_0200430e(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Object_SetPartPalettes(a, Func_02007d28(*p >> 1, 6));
    }
    if (Func_02007d38(*p, 15) == 0) {
        Func_0200434e(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_ForwardValue81fc(s32 a)
{
    Func_020081fc(a);
}

void FieldScene_RunStep6(void)
{
    Actor_Get(6);
    Func_02004476();
}

void FieldScene_RunSetupSequence35c4(void)
{
    Audio_PlayCue(187);
    ColorBuffer_ApplyTarget(0x7fff, 1);
    ColorBuffer_Interpolate(1);
    Task_Wait(4);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
}

/* An effect that circles an actor. */
union OrbitEffect {
    s32 words[26];
    struct {
        u8 unknown_00[8];
        s32 x;
        s32 y;
        s32 z;
        u8 unknown_14[0x1c];
        s32 radius;
        u8 unknown_34[4];
        s32 saved_x;
        s32 unknown_3c;
        s32 saved_z;
        u8 unknown_44[0x20];
        u16 angle;
    } orbit;
};

enum {
    ORBIT_CENTER_ACTOR = 24
};

/*
 * Places the effect on an ellipse around actor 24, its radius plus 3 across
 * and two sine units deep, keeps a copy of the new position, then steps the
 * angle back by a thirty-second of a turn.
 */
void SceneEffect_UpdateOrbitAroundActor(union OrbitEffect *effect)
{
    struct FieldActor *center = Actor_Get(ORBIT_CENTER_ACTOR);
    u16 angle = effect->orbit.angle;

    effect->orbit.x = center->x.fixed + Math_Cos(angle) * (effect->orbit.radius + 3);
    effect->orbit.z = center->z.fixed + (Math_Sin(angle) << 1);
    effect->orbit.saved_x = effect->orbit.x;
    effect->orbit.saved_z = effect->orbit.z;
    effect->orbit.angle -= 0x800;
}

/*
 * Per-frame orbit step for one actor: read the binary angle at +100, place
 * the actor on a circle around scene record 23, mirror the placement into
 * +56/+64, and advance the angle by -0x800, a thirty-second of a turn. The
 * two arms use deliberately different radius terms -- the +98 counter enters
 * both -- and must not be unified. The actor layout is raw offsets: nothing
 * establishes which of +8 and +16 is which world axis, so they are unnamed.
 */
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor)
{
    u8 *anchor = Actor_Get(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Math_Cos(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Math_Sin(angle);
    across = *(s32 *)(anchor + 16)
           + sine *(*(u8 *)(actor + 98) + 4);
    *(s32 *)(actor + 16) = across;

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    {
        s32 next = *pangle;
        next = next + (s32)0xfffff800;
        *pangle = (u16)next;
    }
}

void FieldScene_RunThreeStepsInBracket(void)
{
    Event_Begin();
    FieldScene_RunPairDefeat();
    Func_02007768();
    Func_02007b0c();
    Event_End();
}

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void FieldScene_RunBracketedSceneWithFlag282(void)
{
    extern u8 *Data_03001ebc;

    u8 *workspace;

    Event_Begin();
    FieldScene_RestageParty();
    Func_02008410();
    /* The flag id is built as 141 << 1 rather than folded. */
    GameFlag_Set(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(1);
    Event_End();
}

void FieldScene_RunScene3c9_02003924(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec4;
    s32 record;
    u8 *work;

    rec4 = Value1(Engine_ActorGet, 0);
    Event_Begin();
    *(u8 *)(Func_02009788() + 85) = 0;
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Task_Wait(1);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Actor_Destroy(20);
    Actor_Destroy(19);
    Actor_SetAnimation(0, 19);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    Func_020090a2(rec4);
    Actor_SetAnimation(1, 18);
    record = Actor_Get(1);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(1);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    Func_020090d4();
    Actor_SetAnimation(2, 18);
    record = Actor_Get(2);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(2);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009106();
    Actor_SetAnimation(3, 18);
    record = Actor_Get(3);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(3);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009134();
    Actor_SetPosition(21, 0xc40000, 0xdc0000);
    Actor_SetAnimation(21, 5);
    Actor_SetPosition(6, 0xbc0000, 0x13c0000);
    Call2((void (*)())Engine_ActorSetAnimation, 6, 5);
    record = Actor_Get(6);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(8);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200917e();
    record = Value1(Engine_ActorGet, 9);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200918e();
    record = Actor_Get(10);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091a2();
    record = Value1(Engine_ActorGet, 11);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091b2();
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    record = Actor_Get(23);
    Actor_SetSpriteFlags(record, 0);
    *(u8 *)(Func_02009898(23) + 85) = 4;
    Actor_SetChildValue(23, 4);
    record = Actor_Get(23);
    *(s32 *)(record + 12) = 0x280000;
    Call2(Engine_TaskAddCallback, 0x200da29, 0xc80);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x200;
    *(s32 *)(work + 0x1c8) = 24;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Func_02008784();
    GameFlag_Set(0x9a7);
    Event_RequestExit(2);
}

enum {
    ACTOR_FIRST_OF_PAIR = 20,
    ACTOR_SECOND_OF_PAIR = 19,
    MSG_PAIR_DEFEATED = 0x2809
};

void SceneEffect_SpawnParticlesAboveActor(void);

/*
 * The pair's defeat. Actors 0 to 3 are placed facing northwest, actors 21
 * and 6 between north and northeast, and the pair between south and
 * southeast. Actors 24 and 25 are prepared, the particle task starts and the
 * screen opens. The pair speak, then each falls away in three poses and is
 * removed.
 */
void FieldScene_RunPairDefeat(void)
{
    struct FieldActor *actor;

    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(0)->facing = FACING_NORTHWEST;
    Actor_Get(1)->facing = FACING_NORTHWEST;
    Actor_SetPosition(1, PIXELS(328), PIXELS(168));
    Actor_Get(2)->facing = FACING_NORTHWEST;
    Actor_SetPosition(2, PIXELS(340), PIXELS(196));
    Actor_Get(3)->facing = FACING_NORTHWEST;
    Actor_SetPosition(3, PIXELS(326), PIXELS(204));
    Actor_Get(21)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(21, PIXELS(200), PIXELS(216));
    Actor_Get(6)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(6, PIXELS(200), PIXELS(216));
    Actor_Get(ACTOR_FIRST_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(310), PIXELS(158));
    Actor_Get(ACTOR_SECOND_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(292), PIXELS(158));

    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    Actor_SetSpritePriority(24, 1);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(2);
    actor->z.fixed = PIXELS(96);

    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    Actor_SetSpritePriority(25, 1);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(34);
    actor->z.fixed = PIXELS(96);

    Task_AddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    MapRender_SetValues(0x10000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    Event_Wait(10);
    Event_SetMessage(MSG_PAIR_DEFEATED);
    State_ApplyArgMode0AndSet10(ACTOR_FIRST_OF_PAIR);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 40);
    Audio_PlayCue(17);

    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->x.fixed = PIXELS(308);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 10);
    Event_Wait(20);
    actor->x.fixed = PIXELS(306);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 11);
    Event_Wait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(21);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 12);
    Event_Wait(8);
    Actor_Destroy(ACTOR_FIRST_OF_PAIR);

    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->x.fixed = PIXELS(294);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 8);
    Event_Wait(20);
    actor->x.fixed = PIXELS(300);
    actor->y.fixed = PIXELS(27);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 9);
    Event_Wait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(17);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 10);
    Event_Wait(8);
    Actor_Destroy(ACTOR_SECOND_OF_PAIR);
    Event_Wait(160);
}

void FieldScene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(20);
    Event_OpenMessage(2, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Func_0200478e(0, 0x4000);
    count_flag = 0;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(2, 4);
        count_flag = 1;
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(2, 1);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020047be(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    request_a = 0x1001;
    Actor_WalkToAndWait(1, 0x141, 174);
    Func_02004808(1, 0x2000);
    Func_020047fa(request_a);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_FaceDirection(2, 0xa000, 0);
    Func_02004834(3, 0xc000);
    Func_02004842(1, 0x4000);
    Func_02004834_a(request_a);
    Actor_ShowEmote(1, 0x105, 40);
    Actor_FaceDirection(1, 0x2000, 20);
    Func_0200484e(request_a);
    Actor_FaceDirection(1, 0xa000, 40);
    Event_ShowMessageAndWait(0x8001, 0, 40);
    Actor_FaceDirection(1, 0x2000, 20);
    Func_02004874(request_a);
    Audio_PlayCue(17);
    Actor_SetAnimation(3, 4);
    Func_02004888(3);
    Actor_FaceDirection(1, 0x4000, 80);
    Actor_FaceDirection(3, 0x6000, 80);
    Actor_ShowEmote(3, 0x100, 40);
    Actor_FaceDirection(0, 0x6000, 60);
    Actor_RunRepeatedMotion(0, 2);
    Func_020048da(2, 0xc000);
    Actor_ShowEmote(2, 0x101, 60);
    Actor_FaceDirection(2, 0x6000, 40);
    Actor_Jump(2, 4, 60);
    Func_02004902(1, 0x2000);
    Actor_ShowEmote(1, 0x101, 40);
    Func_020048fe(request_a);
    Actor_FaceDirection(1, 0x6000, 40);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_Jump(1, 4, 40);
    Func_02004926(1);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_SetDestination(21, 200, 188);
    Actor_SetDestination(6, 200, 204);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0xfc0000, 0, 0xbe0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Audio_PlayCue(23);
    Actor_SetAttachedEffect(21, 0x102);
    Func_0200497c(21);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    Func_02004990(21);
    Actor_SetAnimationAndWait(21, 4);
    Func_0200499e(21);
    Actor_FaceDirection(21, 0x3000, 20);
    Func_020049b0(21);
    Actor_SetAttachedEffect(6, 0x102);
    request_b = 0x2003;
    Event_Wait(20);
    Actor_StartRepeatedMotion(3, 2);
    Func_020049d0(request_b);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Func_020049e6(0x2002);
    Func_02004a04(21, 0xe000);
    Actor_RunRepeatedMotion(1, 1);
    Func_020049fe(1);
    Actor_RunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(21, 0, 20);
    Func_02004a16(request_b);
    Actor_SetAnimationAndWait(21, 4);
    Func_02004a24(21);
    Actor_SetAttachedEffect(2, 0x102);
    Func_02004a34(0x2002);
    Actor_SetAnimation(21, 3);
    Func_02004a42(21);
    Actor_ShowEmote(1, 0x103, 40);
    Actor_StartRepeatedMotion(1, 2);
    Func_02004a5a(1);
    Actor_SetAnimation(21, 4);
    Func_02004a68(21);
    Actor_FaceDirection(21, 0x5000, 20);
    Actor_Jump(3, 4, 20);
    Event_ShowMessageAndWait(request_b, 0, 20);
    Actor_ShowEmote(21, 0x103, 40);
}

enum {
    PARTICLE_SOURCE_ACTOR = 23
};

void SceneEffect_SpawnParticlesBesideActor(void);

/*
 * Restages the aerie after the pair's defeat: both of the pair are removed,
 * map cells are copied, the camera and actors are refreshed, and actors 0 to
 * 3 are placed with their rise stopped. The rise counters of actors 21 and 6
 * are cleared, the particle task for actor 23 starts, and the palette is
 * blended in from white over 40 frames.
 */
void FieldScene_RestageParty(void)
{
    struct FieldActor *actor;

    Actor_Destroy(ACTOR_FIRST_OF_PAIR);
    Actor_Destroy(ACTOR_SECOND_OF_PAIR);
    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    MapRender_SetValues(0x20000, 0x20000, 0x10000);
    Camera_MoveTo(-1, -1, -1, 0);
    Actors_Refresh();
    Map_Redraw();
    Task_Wait(1);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    Actor_SetAnimation(0, 19);
    Actor_SetAnimation(1, 18);
    Actor_SetAnimation(2, 18);
    Actor_SetAnimation(3, 18);
    Actor_SetSpriteFlags(Actor_Get(0), 0);
    Actor_SetSpriteFlags(Actor_Get(1), 0);
    Actor_SetSpriteFlags(Actor_Get(2), 0);
    Actor_SetSpriteFlags(Actor_Get(3), 0);

    actor = Actor_Get(0);
    actor->x.fixed = PIXELS(346);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(205);
    Actor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(1);
    actor->x.fixed = PIXELS(356);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(192);
    Actor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(2);
    actor->x.fixed = PIXELS(360);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    Actor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(3);
    actor->x.fixed = PIXELS(334);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    Actor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    Actor_Get(21)->rise_counter = 0;
    Actor_Get(6)->rise_counter = 0;
    Actor_Get(PARTICLE_SOURCE_ACTOR)->motion_flags |= 4;
    Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
    Task_AddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
    gEventWork->transition_frames = 1;
    Event_OpenScreen();
    ColorBuffer_ApplySource(0x7fff, 0);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Task_Wait(60);
}

void FieldScene_RunScene3c9_02004b28(void)
{
    u32 i;
    s32 record;

    Event_SetMessage(0x2829);
    Func_020053c8(21);
    Audio_PlayCue(62);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Camera_SetSpeed(0x4cccc, 0x9999);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(0x2015, 0, 40);
    Actor_RunRepeatedMotion(6, 3);
    Func_0200542c(6);
    Actor_SetAttachedEffect(21, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(0x2015, 0, 80);
    Actor_SetAttachedEffect(6, 0x102);
    Event_Wait(40);
    Actor_StartRepeatedMotion(6, 2);
    Func_02005464(6);
}

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}

void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        Func_0200b096(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}

void FieldScene_RunScene3c9_02005b90(union FieldObject *object)
{
    struct FieldEffect *anchor;
    s32 spin;

    anchor = *(struct FieldEffect **)((u8 *)object + 104);
    spin = object->effect.spin;
    object->effect.x = anchor->x + Math_Cos(spin) * (*(s32 *)((u8 *)object + 48) + 28);
    object->effect.z = (Math_Sin(spin) << 4) + 0xa40000;
    *(s32 *)((u8 *)object + 56) = object->effect.x;
    *(s32 *)((u8 *)object + 64) = object->effect.z;
    object->effect.spin -= 0x200;
}
