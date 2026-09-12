#include "types.h"
#include "scene.h"

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

void *Effect_unk13_4(s32, s32, s32, s32);

typedef s32(*IwramIntegerSquareRoot)(s32);
Spr *Effect_unk14_4(s32);

void *Effect_unk15_4();

Spr *Effect_unk16_4(s32);

Spr *Effect_unk17_4(void);

Spr *Effect_unk18_4(Spr *);

u8 *Effect_unk19_4(s32);
u8 *Effect_unk20_4(s32);
u8 *Effect_unk21_4(s32);
u8 *Effect_unk22_4(s32);
u8 *Effect_unk23_4(s32);
u8 *Effect_unk24_4(s32);
u8 *Effect_unk25_3(s32);

Spr *Effect_unk26_3(s32);

Spr *Effect_unk27_3(s32);

Spr *Effect_unk28_3(s32);

Spr *Effect_unk29_3(s32);

Spr *Effect_unk30_2(s32);
Spr *Effect_unk31_2(s32);
Spr *Effect_unk32_2(s32);
Spr *Effect_unk33_2(s32);

Spr *Effect_unk34_2(void);

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

void Effect_unk35_2();  /* Effect_unk36_2 */

void Effect_unk37_2();  /* Effect_unk38_2, this overlay's own */

void Effect_unk39_2();  /* Scene_RunScene3c9, this overlay's own */

void Effect_unk40_2();  /* GameFlag_Set */

void Effect_unk41_2();  /* Event_ClearStatus1c6 */

void Effect_unk43();  /* Event_WaitValue1c8Frames */

void Effect_unk45();  /* Effect_unk46 */

void Effect_unk47();  /* BattleFx_FinishAction */

u8 *Effect_unk49(s32 index);   /* scene-record accessor (Scene_GetRecord) */

s32 Effect_unk50(s32 angle);   /* sine of a binary angle (Effect_unk51) */

s32 Effect_unk52(s32 angle);   /* cosine of a binary angle (Effect_unk53) */

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
    u8 *ret = Effect_unk13_4(kind, x, y, z);

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
        *p = Effect_Apply4(Effect_unk7(s), 80) + 80;
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
            s32 t = ((u32)(Effect_unk8() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Effect_unk9() * 80)) >> 16;
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
    ref = Effect_unk14_4(0);
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

    actor = Effect_unk15_4(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Effect_unk54(0x200000, angle, position);
    if (Effect_unk10(actor, position) == 0) {
        Effect_unk55(592);
        Effect_unk56();
        Effect_unk57(actor, 6);
        Effect_unk58(6);
        Effect_unk59(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Effect_unk60(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Effect_unk61(actor, 0);
        Effect_unk62(0, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Effect_unk63(actor, 6);
        Effect_unk64(actor, 1);
        actor->flags55 = flags;
    }
}

void Effect_SpawnAndBobWithActorZero(void)
{
    Spr4 *a = Effect_unk16_4(0);
    Spr4 *b;
    Spr4 *r;
    s32 k;

    Effect_unk65();
    r = Effect_unk17_4();
    gOv5 = r;
    if (r != 0) {
        Effect_unk3_2(592);
        b = Effect_unk18_4(gOv5);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Effect_unk4_2(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Effect_unk5_2(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Effect_unk6_2(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Effect_unk7_2(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Effect_unk66();
}

void Actor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *(Effect_unk19_4(0) + 0x55) = 3;
    val = 4;
    *(Effect_unk20_4(12) + 0x55) = val;
    *(Effect_unk21_4(13) + 0x55) = val;
    *(Effect_unk22_4(14) + 0x55) = val;
    *(Effect_unk23_4(15) + 0x55) = val;
    *(Effect_unk24_4(16) + 0x55) = val;
    *(Effect_unk25_3(17) + 0x55) = val;
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

    Effect_unk8_2(324);
    Effect_unk9_2(1);
    Effect_unk10_2(272);
    Effect_Apply6(Effect_unk26_3(8), 0);
    Effect_Apply7(Effect_unk27_3(9), 0);
    Effect_Apply8(Effect_unk28_3(10), 0);
    Effect_Apply9(Effect_unk29_3(11), 0);
    v = (s32)0xffff0000;
    Effect_unk30_2(10)->unk18 = v;
    Effect_unk31_2(11)->unk18 = v;
    i = 12;
    z = 0;
    do {
        obj = Effect_unk32_2(i);
        Effect_Apply10(Effect_unk33_2(i), z);
        Effect_Apply11(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gCell[225];

    switch (mode) {
    case 1:
        if (Effect_unk11(0x109) == 0) {
            Effect_unk67();
        }
        break;
    case 2:
        Effect_unk68();
        break;
    case 3:
        Effect_unk69();
        break;
    case 0x5d:
        Effect_unk70();
        break;
    case 4:
        Effect_unk71();
        break;
    case 9:
        Effect_unk72();
        if (Effect_unk12(0x345) != 0) {
            Effect_Apply12(0, 65);
        } else if (Effect_unk13(0x346) != 0) {
            Effect_Apply12(1, 65);
        } else if (Effect_unk14(0x347) != 0) {
            Effect_Apply12(2, 65);
        } else {
            Effect_Apply13(3, 65);
        }
        Effect_unk11_2(9);
        break;
    }

    if (Effect_unk15(0x109) != 0) {
        if (Effect_unk16() != 0) {
            Spr5 *p = Effect_unk34_2();
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
    Effect_unk12_2(10);
}

void Scene_CallPairWith10(s32 a, s32 b)
{
    Effect_unk29_2(a, b, 10);
}

void Scene_RunScene3c9(s32 a0, s32 a1)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (a1 != 0) {
        Effect_unk73(a0, 0);
        record = Effect_unk17(a0);
        Effect_unk74(record, 1);
        Effect_Place(a0, 0xcccc, 0x6666);
    } else {
        Effect_unk75(a0, 15);
        record = Effect_unk18(a0);
        Effect_unk76(record, 0);
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
        Effect_unk13_2(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Effect_Apply18(a, Effect_Apply19(*p >> 1, 6));
    }
    if (Effect_Apply20(*p, 15) == 0) {
        Effect_unk14_2(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Effect_unk15_2((s32)effect);
    } else {
        s32 amplitude = Effect_unk19(frame << 10);
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
        Effect_unk16_2((s32)effect);
    } else {
        s32 amplitude = Effect_unk20(frame << 10);
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
    Effect_unk17_2(a);
}

void Scene_RunStep6(void)
{
    extern u8 gWork[];

    Effect_unk18_2(6);
    Effect_unk77();
}

void Scene_RunSetupSequence35c4(void)
{
    extern u8 *gWork;

    Effect_unk19_2(187);
    Effect_Apply22(0x7fff, 1);
    Effect_unk20_2(1);
    Effect_unk21_2(4);
    Effect_Apply23(0x40250d, 1);
    Effect_unk22_2(1);
    Effect_unk23_2(1);
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
    u8 *anchor = Effect_unk49(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Effect_unk52(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Effect_unk50(angle);
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

    Effect_unk78();
    Effect_unk79();
    Effect_unk80();
    Effect_unk81();
    Effect_unk82();
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

    Effect_unk35_2();
    Effect_unk37_2();
    Effect_unk39_2();
    /* The flag id is built as 141 << 1 rather than folded. */
    Effect_unk40_2(141 << 1);

    workspace = gWork;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Effect_unk41_2();
    Effect_unk43();
    Effect_unk45(1);
    Effect_unk47();
}

void Scene_RunScene3c9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec4;
    s32 record;
    u8 *work;

    rec4 = Effect_Check(0);
    Effect_unk83();
    *(u8 *)(Effect_unk21() + 85) = 0;
    Effect_SetRect(102, 4, 74, 4, 18, 23);
    Effect_unk2_5(39, 72, 11, 72, 16, 20);
    Effect_unk3_5(19, 6, 3, 7, 22, 6);
    Effect_unk4_5(19, 6, 3, 7, 13, 6);
    Effect_unk5_5(19, 6, 3, 7, 22, 13);
    Effect_unk6_5(19, 6, 3, 7, 13, 13);
    Effect_unk84(1);
    Effect_Run(0xc00000, -0x400000, 0xee0000, 0);
    Effect_unk85(1);
    Effect_unk86();
    Effect_unk87(1);
    Effect_unk88(20);
    Effect_unk89(19);
    Effect_unk90(0, 19);
    record = Effect_unk22(0);
    Effect_unk91(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    Effect_unk92(rec4);
    Effect_unk93(1, 18);
    record = Effect_unk23(1);
    Effect_unk94(record, 0);
    record = Effect_unk24(1);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    Effect_unk95();
    Effect_unk96(2, 18);
    record = Effect_unk25(2);
    Effect_unk97(record, 0);
    record = Effect_unk26(2);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Effect_unk98();
    Effect_unk99(3, 18);
    record = Effect_unk27(3);
    Effect_unk100(record, 0);
    record = Effect_unk28(3);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Effect_unk101();
    Effect_unk2_3(21, 0xc40000, 0xdc0000);
    Effect_unk102(21, 5);
    Effect_unk3_3(6, 0xbc0000, 0x13c0000);
    Effect_unk2_4(6, 5);
    record = Effect_unk29(6);
    Effect_unk103(record, 0);
    record = Effect_unk30(8);
    *(s32 *)(record + 8) += -0x100000;
    Effect_unk104();
    record = Effect_unk2(9);
    *(s32 *)(record + 8) += -0x100000;
    Effect_unk105();
    record = Effect_unk31(10);
    *(s32 *)(record + 8) += 0x100000;
    Effect_unk106();
    record = Effect_unk3(11);
    *(s32 *)(record + 8) += 0x100000;
    Effect_unk107();
    Effect_unk4_3(0x10000, 0x10000, 0x10000);
    record = Effect_unk32(23);
    Effect_unk108(record, 0);
    *(u8 *)(Effect_unk33(23) + 85) = 4;
    Effect_unk109(23, 4);
    record = Effect_unk34(23);
    *(s32 *)(record + 12) = 0x280000;
    Effect_unk4(0x200da29, 0xc80);
    work = *(u8 *volatile *)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x200;
    *(s32 *)(((s32)work + 0x1c8)) = 24;
    Effect_unk110();
    Effect_unk111();
    Effect_unk112(40);
    Effect_unk113();
    Effect_Do(0x9a7);
    Effect_unk114(2);
}

void Scene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Effect_unk115(1, 1);
    Effect_unk116(20);
    Effect_unk117(1, 0, 20);
    Effect_unk118(2, 1);
    Effect_unk119(20);
    Effect_unk5(2, 0);
    Effect_unk5_3(2, 0xc000, 0);
    Effect_unk120(0, 0x4000);
    count_flag = 0;
    if (Effect_unk6(0, 0) == 0) {
        Effect_unk121(20);
        Effect_unk122(2, 4);
        count_flag = 1;
    } else {
        Effect_unk123(20);
        Effect_unk124(2, 1);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_unk125(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_unk6_3(1, 0x19999, 0xcccc);
    request_a = 0x1001;
    Effect_unk7_3(1, 0x141, 174);
    Effect_unk126(1, 0x2000);
    Effect_unk127(request_a);
    Effect_unk8_3(0, 0xa000, 0);
    Effect_unk9_3(2, 0xa000, 0);
    Effect_unk128(3, 0xc000);
    Effect_unk129(1, 0x4000);
    Effect_unk130(request_a);
    Effect_unk10_3(1, 0x105, 40);
    Effect_unk131(1, 0x2000, 20);
    Effect_unk132(request_a);
    Effect_unk11_3(1, 0xa000, 40);
    Effect_unk12_3(0x8001, 0, 40);
    Effect_unk133(1, 0x2000, 20);
    Effect_unk134(request_a);
    Effect_unk135(17);
    Effect_unk136(3, 4);
    Effect_unk137(3);
    Effect_unk138(1, 0x4000, 80);
    Effect_unk13_3(3, 0x6000, 80);
    Effect_unk14_3(3, 0x100, 40);
    Effect_unk15_3(0, 0x6000, 60);
    Effect_unk139(0, 2);
    Effect_unk140(2, 0xc000);
    Effect_unk16_3(2, 0x101, 60);
    Effect_unk17_3(2, 0x6000, 40);
    Effect_unk141(2, 4, 60);
    Effect_unk142(1, 0x2000);
    Effect_unk18_3(1, 0x101, 40);
    Effect_unk143(request_a);
    Effect_unk19_3(1, 0x6000, 40);
    Effect_unk20_3(1, 0x100, 0);
    Effect_unk144(1, 4, 40);
    Effect_unk145(1);
    Effect_unk21_3(21, 0xcccc, 0x6666);
    Effect_unk146(21, 200, 188);
    Effect_unk147(6, 200, 204);
    Effect_unk3_4(0x33333, 0x6666);
    Effect_unk4_4(0xfc0000, 0, 0xbe0000, 1);
    Effect_unk148();
    Effect_unk149(40);
    Effect_unk150(23);
    Effect_unk5_4(21, 0x102);
    Effect_unk151(21);
    Effect_unk152(21, 1);
    Effect_unk153(20);
    Effect_unk154(21);
    Effect_unk155(21, 4);
    Effect_unk156(21);
    Effect_unk22_3(21, 0x3000, 20);
    Effect_unk157(21);
    Effect_unk6_4(6, 0x102);
    request_b = 0x2003;
    Effect_unk158(20);
    Effect_unk159(3, 2);
    Effect_unk160(request_b);
    Effect_unk161(21, 2);
    Effect_unk162(20);
    Effect_unk163(0x2002);
    Effect_unk164(21, 0xe000);
    Effect_unk165(1, 1);
    Effect_unk166(1);
    Effect_unk167(21, 1);
    Effect_unk168(21, 0, 20);
    Effect_unk169(request_b);
    Effect_unk170(21, 4);
    Effect_unk171(21);
    Effect_unk7_4(2, 0x102);
    Effect_unk172(0x2002);
    Effect_unk173(21, 3);
    Effect_unk174(21);
    Effect_unk23_3(1, 0x103, 40);
    Effect_unk175(1, 2);
    Effect_unk176(1);
    Effect_unk177(21, 4);
    Effect_unk178(21);
    Effect_unk24_3(21, 0x5000, 20);
    Effect_unk179(3, 4, 20);
    Effect_unk180(request_b, 0, 20);
    Effect_unk25_2(21, 0x103, 40);
}

void Scene_RunScene3c9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_unk2_2(0x2829);
    Effect_unk181(21);
    Effect_unk182(62);
    Effect_unk26_2(0x10000, 0x10000, 0x10000);
    Effect_unk8_4(0x4cccc, 0x9999);
    Effect_unk9_4(0x40000, 0x8000);
    Effect_unk10_4(0xc00000, -0x400000, 0xee0000, 1);
    Effect_unk183();
    Effect_unk184(40);
    Effect_unk185(21, 1);
    Effect_unk27_2(0x2015, 0, 40);
    Effect_unk186(6, 3);
    Effect_unk187(6);
    Effect_unk11_4(21, 0x102);
    Effect_unk188(60);
    Effect_unk28_2(0x2015, 0, 80);
    Effect_unk12_4(6, 0x102);
    Effect_unk189(40);
    Effect_unk190(6, 2);
    Effect_unk191(6);
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

        Effect_unk24_2(obj);

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
    record = Effect_unk35(p8);
    *(volatile s32 *)(a0 + 8) = (*(s32 *)(p10 + 8) + (record *(*(s32 *)(a0 + 48) + 28)));
    value = Effect_unk36(p8);
    *(volatile s32 *)(a0 + 16) = ((value << 4) + 0xa40000);
    *(volatile s32 *)(a0 + 56) = *(s32 *)(a0 + 8);
    *(volatile s32 *)(a0 + 64) = ((value << 4) + 0xa40000);
    p8b = *(volatile u16 *)(a0 + 100);
    p8b = p8b + -0x200;
    *(volatile u16 *)(a0 + 100) = (u16)p8b;
}
