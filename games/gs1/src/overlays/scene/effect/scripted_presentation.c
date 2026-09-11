#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/effect/scripted_presentation.h"

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

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
} Obj;

typedef struct {
    u8 filler0[0x50];
    Obj *obj;
} Spr;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr2;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj2;

typedef struct {
    u8 filler0[0x50];
    Obj2 *obj;
} Spr3;

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
} Spr4;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr5;

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

extern unsigned char gOv[];
extern unsigned char gOv2[];
extern unsigned char gOv3[];
extern unsigned char gOv4[];
extern Spr *gOv5;
extern s16 gCell[];

void *Effect_Run13(s32, s32, s32, s32);

typedef s32(*IwramIntegerSquareRoot)(s32);
Spr *Effect_Run14(s32);

void *Effect_Run15();

Spr *Effect_Run16(s32);

Spr *Effect_Run17(void);

Spr *Effect_Run18(Spr *);

u8 *Effect_Run19(s32);
u8 *Effect_Run20(s32);
u8 *Effect_Run21(s32);
u8 *Effect_Run22(s32);
u8 *Effect_Run23(s32);
u8 *Effect_Run24(s32);
u8 *Effect_Run25(s32);

Spr *Effect_Run26(s32);

Spr *Effect_Run27(s32);

Spr *Effect_Run28(s32);

Spr *Effect_Run29(s32);

Spr *Effect_Run30(s32);
Spr *Effect_Run31(s32);
Spr *Effect_Run32(s32);
Spr *Effect_Run33(s32);

Spr *Effect_Run34(void);

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

void Effect_Run35();  /* Effect_Run36 */

void Effect_Run37();  /* Effect_Run38, this overlay's own */

void Effect_Run39();  /* Scene_RunScene3c9, this overlay's own */

void Effect_Run40();  /* GameFlag_Set */

void Effect_Run41();  /* Effect_Run42 */

void Effect_Run43();  /* Effect_Run44 */

void Effect_Run45();  /* Effect_Run46 */

void Effect_Run47();  /* Effect_Run48 */

u8 *Effect_Run49(s32 index);   /* scene-record accessor (Scene_GetRecord) */

s32 Effect_Run50(s32 angle);   /* sine of a binary angle (Effect_Run51) */

s32 Effect_Run52(s32 angle);   /* cosine of a binary angle (Effect_Run53) */

void OvObj_SetRecordField1(Obj *o, u32 v)
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
void *OvObj_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Effect_Run13(kind, x, y, z);

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
        Effect_Apply(ret, 0);
        Effect_Apply2(ret, 14);
        Effect_Apply3(ret, 1);
        return ret;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void Effect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 OvObj_StepScaleByCounter(Spr *s)
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
        *p = Effect_Apply4(Effect_Check7(s), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

void OvObj_DecayRecordField1e(Spr *s)
{
    Obj *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OvObj_UpdateHeadingTimer(Spr2 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Effect_Check8() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Effect_Check9() * 80)) >> 16;
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
    return (s32)gOv;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)gOv2;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)gOv3;
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

s32 Actor_FindNearestSlotOfKindF2(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Spr3 **p;
    s32 best = 0;
    Spr3 *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Effect_Run14(0);
    i = 8;
    p = (Spr3 **)(work + 0x34);
    do {
        Spr3 *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = Effect_Apply5((u8 *)ref + 8, (u8 *)spr + 8);
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

    actor = Effect_Run15(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Effect_Run54(0x200000, angle, position);
    if (Effect_Check10(actor, position) == 0) {
        Effect_Run55(592);
        Effect_Run56();
        Effect_Run57(actor, 6);
        Effect_Run58(6);
        Effect_Run59(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Effect_Run60(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Effect_Run61(actor, 0);
        Effect_Run62(0, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Effect_Run63(actor, 6);
        Effect_Run64(actor, 1);
        actor->flags55 = flags;
    }
}

void Effect_SpawnAndBobWithActorZero(void)
{
    Spr4 *a = Effect_Run16(0);
    Spr4 *b;
    Spr4 *r;
    s32 k;

    Effect_Run65();
    r = Effect_Run17();
    gOv5 = r;
    if (r != 0) {
        Effect_Do3(592);
        b = Effect_Run18(gOv5);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Effect_Do4(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Effect_Do5(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Effect_Do6(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Effect_Do7(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Effect_Run66();
}

void Actor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *(Effect_Run19(0) + 0x55) = 3;
    val = 4;
    *(Effect_Run20(12) + 0x55) = val;
    *(Effect_Run21(13) + 0x55) = val;
    *(Effect_Run22(14) + 0x55) = val;
    *(Effect_Run23(15) + 0x55) = val;
    *(Effect_Run24(16) + 0x55) = val;
    *(Effect_Run25(17) + 0x55) = val;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)gOv4;
}

s32 Scene_InitActorsAndDispatchBySubstate(void)
{
    extern u8 *gWork;

    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr5 *obj;

    Effect_Do8(324);
    Effect_Do9(1);
    Effect_Do10(272);
    Effect_Apply6(Effect_Run26(8), 0);
    Effect_Apply7(Effect_Run27(9), 0);
    Effect_Apply8(Effect_Run28(10), 0);
    Effect_Apply9(Effect_Run29(11), 0);
    v = (s32)0xffff0000;
    Effect_Run30(10)->unk18 = v;
    Effect_Run31(11)->unk18 = v;
    i = 12;
    z = 0;
    do {
        obj = Effect_Run32(i);
        Effect_Apply10(Effect_Run33(i), z);
        Effect_Apply11(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gCell[225];

    switch (mode) {
    case 1:
        if (Effect_Check11(0x109) == 0) {
            Effect_Run67();
        }
        break;
    case 2:
        Effect_Run68();
        break;
    case 3:
        Effect_Run69();
        break;
    case 0x5d:
        Effect_Run70();
        break;
    case 4:
        Effect_Run71();
        break;
    case 9:
        Effect_Run72();
        if (Effect_Check12(0x345) != 0) {
            Effect_Apply12(0, 65);
        } else if (Effect_Check13(0x346) != 0) {
            Effect_Apply12(1, 65);
        } else if (Effect_Check14(0x347) != 0) {
            Effect_Apply12(2, 65);
        } else {
            Effect_Apply13(3, 65);
        }
        Effect_Do11(9);
        break;
    }

    if (Effect_Check15(0x109) != 0) {
        if (Effect_Check16() != 0) {
            Spr5 *p = Effect_Run34();
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void State_ApplyArgMode0AndSet10(s32 a)
{
    Effect_Apply14(a, 0);
    Effect_Do12(10);
}

void Scene_CallPairWith10(s32 a, s32 b)
{
    Effect_Place29(a, b, 10);
}

void Scene_RunScene3c9(s32 a0, s32 a1)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (a1 != 0) {
        Effect_Run73(a0, 0);
        record = Effect_Check17(a0);
        Effect_Run74(record, 1);
        Effect_Place(a0, 0xcccc, 0x6666);
    } else {
        Effect_Run75(a0, 15);
        record = Effect_Check18(a0);
        Effect_Run76(record, 0);
    }
}

void Effect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)0x03001e40 & 2) {
        Effect_Apply15(a, 7);
    } else {
        Effect_Apply16(a, 0);
    }
    if (Effect_Apply17(*(s32 *)0x03001e40, 15) == 0) {
        Effect_Do13(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Effect_Apply18(a, Effect_Apply19(*p >> 1, 6));
    }
    if (Effect_Apply20(*p, 15) == 0) {
        Effect_Do14(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Effect_Do15((s32)effect);
    } else {
        s32 amplitude = Effect_Check19(frame << 10);
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
        Effect_Do16((s32)effect);
    } else {
        s32 amplitude = Effect_Check20(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

void State_ApplyPair140And0(void)
{
    Effect_Apply21(140, 0);
}

void Scene_ForwardValue81fc(s32 a)
{
    Effect_Do17(a);
}

void Scene_RunStep6(void)
{
    extern u8 gWork[];

    Effect_Do18(6);
    Effect_Run77();
}

void Scene_RunSetupSequence35c4(void)
{
    extern u8 *gWork;

    Effect_Do19(187);
    Effect_Apply22(0x7fff, 1);
    Effect_Do20(1);
    Effect_Do21(4);
    Effect_Apply23(0x40250d, 1);
    Effect_Do22(1);
    Effect_Do23(1);
}

/*
 * Per-frame orbit step for one actor: read the binary angle at +100, place
 * the actor on a circle around scene record 23, mirror the placement into
 * +56/+64, and advance the angle by -0x800, a thirty-second of a turn. The
 * two arms use deliberately different radius terms -- the +98 counter enters
 * both -- and must not be unified. The actor layout is raw offsets: nothing
 * establishes which of +8 and +16 is which world axis, so they are unnamed.
 */
void Effect_UpdateCounterDrivenOrbit(u8 *actor)
{
    u8 *anchor = Effect_Run49(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Effect_Run52(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Effect_Run50(angle);
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

void Scene_RunThreeStepsInBracket(void)
{
    extern u8 *gWork;

    Effect_Run78();
    Effect_Run79();
    Effect_Run80();
    Effect_Run81();
    Effect_Run82();
}

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void Scene_RunBracketedSceneWithFlag282(void)
{
    extern u8 *gWork;

    u8 *workspace;

    Effect_Run35();
    Effect_Run37();
    Effect_Run39();
    /* The flag id is built as 141 << 1 rather than folded. */
    Effect_Run40(141 << 1);

    workspace = gWork;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Effect_Run41();
    Effect_Run43();
    Effect_Run45(1);
    Effect_Run47();
}

void Scene_RunScene3c9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec4;
    s32 record;
    u8 *work;

    rec4 = Effect_Check(0);
    Effect_Run83();
    *(u8 *)(Effect_Check21() + 85) = 0;
    Effect_SetRect(102, 4, 74, 4, 18, 23);
    Effect_SetRect2(39, 72, 11, 72, 16, 20);
    Effect_SetRect3(19, 6, 3, 7, 22, 6);
    Effect_SetRect4(19, 6, 3, 7, 13, 6);
    Effect_SetRect5(19, 6, 3, 7, 22, 13);
    Effect_SetRect6(19, 6, 3, 7, 13, 13);
    Effect_Run84(1);
    Effect_Run(0xc00000, -0x400000, 0xee0000, 0);
    Effect_Run85(1);
    Effect_Run86();
    Effect_Run87(1);
    Effect_Run88(20);
    Effect_Run89(19);
    Effect_Run90(0, 19);
    record = Effect_Check22(0);
    Effect_Run91(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    Effect_Run92(rec4);
    Effect_Run93(1, 18);
    record = Effect_Check23(1);
    Effect_Run94(record, 0);
    record = Effect_Check24(1);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    Effect_Run95();
    Effect_Run96(2, 18);
    record = Effect_Check25(2);
    Effect_Run97(record, 0);
    record = Effect_Check26(2);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Effect_Run98();
    Effect_Run99(3, 18);
    record = Effect_Check27(3);
    Effect_Run100(record, 0);
    record = Effect_Check28(3);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Effect_Run101();
    Effect_Place2(21, 0xc40000, 0xdc0000);
    Effect_Run102(21, 5);
    Effect_Place3(6, 0xbc0000, 0x13c0000);
    Effect_Run2(6, 5);
    record = Effect_Check29(6);
    Effect_Run103(record, 0);
    record = Effect_Check30(8);
    *(s32 *)(record + 8) += -0x100000;
    Effect_Run104();
    record = Effect_Check2(9);
    *(s32 *)(record + 8) += -0x100000;
    Effect_Run105();
    record = Effect_Check31(10);
    *(s32 *)(record + 8) += 0x100000;
    Effect_Run106();
    record = Effect_Check3(11);
    *(s32 *)(record + 8) += 0x100000;
    Effect_Run107();
    Effect_Place4(0x10000, 0x10000, 0x10000);
    record = Effect_Check32(23);
    Effect_Run108(record, 0);
    *(u8 *)(Effect_Check33(23) + 85) = 4;
    Effect_Run109(23, 4);
    record = Effect_Check34(23);
    *(s32 *)(record + 12) = 0x280000;
    Effect_Check4(0x200da29, 0xc80);
    work = *(u8 *volatile *)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x200;
    *(s32 *)(((s32)work + 0x1c8)) = 24;
    Effect_Run110();
    Effect_Run111();
    Effect_Run112(40);
    Effect_Run113();
    Effect_Do(0x9a7);
    Effect_Run114(2);
}

void Scene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Effect_Run115(1, 1);
    Effect_Run116(20);
    Effect_Run117(1, 0, 20);
    Effect_Run118(2, 1);
    Effect_Run119(20);
    Effect_Check5(2, 0);
    Effect_Place5(2, 0xc000, 0);
    Effect_Run120(0, 0x4000);
    count_flag = 0;
    if (Effect_Check6(0, 0) == 0) {
        Effect_Run121(20);
        Effect_Run122(2, 4);
        count_flag = 1;
    } else {
        Effect_Run123(20);
        Effect_Run124(2, 1);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_Run125(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_Place6(1, 0x19999, 0xcccc);
    request_a = 0x1001;
    Effect_Place7(1, 0x141, 174);
    Effect_Run126(1, 0x2000);
    Effect_Run127(request_a);
    Effect_Place8(0, 0xa000, 0);
    Effect_Place9(2, 0xa000, 0);
    Effect_Run128(3, 0xc000);
    Effect_Run129(1, 0x4000);
    Effect_Run130(request_a);
    Effect_Place10(1, 0x105, 40);
    Effect_Run131(1, 0x2000, 20);
    Effect_Run132(request_a);
    Effect_Place11(1, 0xa000, 40);
    Effect_Place12(0x8001, 0, 40);
    Effect_Run133(1, 0x2000, 20);
    Effect_Run134(request_a);
    Effect_Run135(17);
    Effect_Run136(3, 4);
    Effect_Run137(3);
    Effect_Run138(1, 0x4000, 80);
    Effect_Place13(3, 0x6000, 80);
    Effect_Place14(3, 0x100, 40);
    Effect_Place15(0, 0x6000, 60);
    Effect_Run139(0, 2);
    Effect_Run140(2, 0xc000);
    Effect_Place16(2, 0x101, 60);
    Effect_Place17(2, 0x6000, 40);
    Effect_Run141(2, 4, 60);
    Effect_Run142(1, 0x2000);
    Effect_Place18(1, 0x101, 40);
    Effect_Run143(request_a);
    Effect_Place19(1, 0x6000, 40);
    Effect_Place20(1, 0x100, 0);
    Effect_Run144(1, 4, 40);
    Effect_Run145(1);
    Effect_Place21(21, 0xcccc, 0x6666);
    Effect_Run146(21, 200, 188);
    Effect_Run147(6, 200, 204);
    Effect_Run3(0x33333, 0x6666);
    Effect_Run4(0xfc0000, 0, 0xbe0000, 1);
    Effect_Run148();
    Effect_Run149(40);
    Effect_Run150(23);
    Effect_Run5(21, 0x102);
    Effect_Run151(21);
    Effect_Run152(21, 1);
    Effect_Run153(20);
    Effect_Run154(21);
    Effect_Run155(21, 4);
    Effect_Run156(21);
    Effect_Place22(21, 0x3000, 20);
    Effect_Run157(21);
    Effect_Run6(6, 0x102);
    request_b = 0x2003;
    Effect_Run158(20);
    Effect_Run159(3, 2);
    Effect_Run160(request_b);
    Effect_Run161(21, 2);
    Effect_Run162(20);
    Effect_Run163(0x2002);
    Effect_Run164(21, 0xe000);
    Effect_Run165(1, 1);
    Effect_Run166(1);
    Effect_Run167(21, 1);
    Effect_Run168(21, 0, 20);
    Effect_Run169(request_b);
    Effect_Run170(21, 4);
    Effect_Run171(21);
    Effect_Run7(2, 0x102);
    Effect_Run172(0x2002);
    Effect_Run173(21, 3);
    Effect_Run174(21);
    Effect_Place23(1, 0x103, 40);
    Effect_Run175(1, 2);
    Effect_Run176(1);
    Effect_Run177(21, 4);
    Effect_Run178(21);
    Effect_Place24(21, 0x5000, 20);
    Effect_Run179(3, 4, 20);
    Effect_Run180(request_b, 0, 20);
    Effect_Place25(21, 0x103, 40);
}

void Scene_RunScene3c9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_Do2(0x2829);
    Effect_Run181(21);
    Effect_Run182(62);
    Effect_Place26(0x10000, 0x10000, 0x10000);
    Effect_Run8(0x4cccc, 0x9999);
    Effect_Run9(0x40000, 0x8000);
    Effect_Run10(0xc00000, -0x400000, 0xee0000, 1);
    Effect_Run183();
    Effect_Run184(40);
    Effect_Run185(21, 1);
    Effect_Place27(0x2015, 0, 40);
    Effect_Run186(6, 3);
    Effect_Run187(6);
    Effect_Run11(21, 0x102);
    Effect_Run188(60);
    Effect_Place28(0x2015, 0, 80);
    Effect_Run12(6, 0x102);
    Effect_Run189(40);
    Effect_Run190(6, 2);
    Effect_Run191(6);
}

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void Actor_ParkRecord(u8 *record)
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

void Effect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        Effect_Do24(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}

void Scene_RunScene3c9(u8 *a0)
{
    extern u8 gWork[];

    s32 p10;
    s32 p8;
    s32 p8b;
    s32 record;
    s32 value;

    p10 = *(s32 *)(a0 + 104);
    p8 = *(u16 *)(a0 + 100);
    record = Effect_Check35(p8);
    *(volatile s32 *)(a0 + 8) = (*(s32 *)(p10 + 8) + (record *(*(s32 *)(a0 + 48) + 28)));
    value = Effect_Check36(p8);
    *(volatile s32 *)(a0 + 16) = ((value << 4) + 0xa40000);
    *(volatile s32 *)(a0 + 56) = *(s32 *)(a0 + 8);
    *(volatile s32 *)(a0 + 64) = ((value << 4) + 0xa40000);
    p8b = *(volatile u16 *)(a0 + 100);
    p8b = p8b + -0x200;
    *(volatile u16 *)(a0 + 100) = (u16)p8b;
}
