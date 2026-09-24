#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#define OverlayObject_IntegrateVelocities Effect_Move

#define OverlayObject_IntegrateVelocities Func_02000ab0

#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

struct Actor_02000cc0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

typedef struct Actor_02000cd0 {
    u8 pad0[6];
    u16 tag;               /* +6  */
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
    u8 pad20[20];
    s32 speedZ;            /* +40 */
    s32 pad44;
    s32 speedX;            /* +48 */
    s32 speedY;            /* +52 */
    u8 pad56[29];
    u8 flags;              /* +85 */
} Actor_02000cd0;

typedef struct Target_02000cd0 {
    s32 x;                 /* +0, 12.20 fixed point */
    s32 pad4;
    s32 z;                 /* +8, 12.20 fixed point */
} Target_02000cd0;

struct Actor_02000dc8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

/*
 * resource_3c4: land an actor on the highest platform sharing its tile.
 */
typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

struct Actor_02000ec8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flatla3; };

struct Track02001038 {
    u8 head[12];
    s32 value;
    u8 gap16[4];
    s32 limit;
    u8 gap24[16];
    s32 state;
    u8 gap44[16];
    s32 mark;
};

/*
 * Per-frame integrator for an effect record -- resource_3c4. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

struct Actor02001424 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Actor_02001510 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Data_0200b474[];
extern u8 Data_0200b654[];
extern u8 Data_0200b42c[];
extern u8 Data_0200b81c[];
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

struct SceneObject *Func_020042fa(void);
void Func_02001d32(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
void Func_020036da(void);
void Func_02003894(void);
u8 *Func_0200457c(s32);
u8 *Func_02004588(s32);
u8 *Func_02004598(s32);
void Func_02003072(void);
u8 *Func_02004a62(s32);
Slot_02001f70 *Func_02005086(s32 slot);
Slot_02001f70 *Func_020050a2(s32 slot);
void Func_020044dc(void);
s32 Func_02003d6a();
s32 Func_02003dca_a();
s32 Func_02003dd6();
Record *Func_0200410a(s32);
Record *Func_02004118(s32);
Record *Func_02004126(s32);
Slot_02001c2c *Func_02004d56();
Slot_02001c2c *Func_02004d5e();
Slot_02001c2c *Func_02004d6e();
Slot_02001c2c *Func_02004d76();
Slot_02001c2c *Func_02004d86();
Slot_02001c2c *Func_02004d98();
Slot_02001c2c *Func_02004dda();
void Func_02004d4e();
void Func_02002ecc();
void Func_02004402(u8 *);
void Func_020037a6(void);
void Func_020043c6(void);
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02004502(s32, s32, s32);
void Func_020020de();
void Func_020022dc();
void Func_02004842();
void Func_02002d82();
void Func_020039d4();
s32 Func_02004f26();
s32 Func_02004f3c();
s32 Func_02004f6a();
s32 Func_02004f7e();
s32 Func_02004f96();
s32 Func_02004fe2();
s32 Func_02004ff0();
s32 Func_02004ffe();
s32 Func_0200500c();
s32 Func_0200502c();
void Func_0200519e();
s32 Func_020055ba();
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_02004836(s32);
void Func_0200304c(void);
u8 *Func_02004b48();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
u8 *Func_02004b84_b();
struct Record_02000ec8;
s32 Func_02008ec8(struct Record_02000ec8 *record);
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Copy the low two mode bits into the object's owner record. */

/*
 * Actor movement for overlay resource_3c4.  Declarations are left without
 * prototypes because Func_02003dec and Func_02003dca are each called twice
 * with different argument shapes.
 */

/*
 * Slot nine setup for overlay resource_3c4.
 */

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Func_02004b84 serves two sites with different argument shapes
 * and is declared without a prototype.
 */

/*
 * Setup step in resource_3c4. One six-argument placement, then two identical
 * follow-ups that pin an overlay at the 12.20 grid cell of slots 8 and 9: the
 * slot's +8 and +16 words are each shifted right by 20 and passed as the two
 * stack arguments of the placement call.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ s32 Value1_02001d04(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call1_020020a4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value1_02002480(s32 (*f)(), s32 a0)
{
    return f(a0);
}

struct Record_02000ec8;

void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */
u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Object_Create(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 14);
        Object_SetBlendMode(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

/*
 * Spawn an object and prepare its owner record and flags.  The three callees
 * are declared at their in-overlay entry points, not at veneers.  Each
 * read-modify-write takes its mask from a local that is initialised first and
 * ANDed with the loaded byte afterwards, which is what puts the mask in r3 and
 * the byte in r2.  The +0x23 mask stays byte-wide as 0xfe rather than ~1,
 * which would widen it to a negated 2.
 */
u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Object_Create(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Actor_SetSpriteFlags(no, 0);
    return 0;
}

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Actor_Get(0);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_02003d6a(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Func_02003dca_a(actor, probe) == 1) {
        goto refuse;
    }
    if (Func_02003dd6(actor, target) != 0) {
        goto refuse;
    }

    Event_Begin();
    Object_SetAnimation(actor, 6);
    Task_Wait(6);
    Audio_PlayCue(152);
    Object_SetAnimation(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Object_SetAnimation(actor, 6);
    Actor_SetSpriteFlags(actor, 1);
    Task_Wait(6);

    actor->flags = saved;
    Event_End();
    return 0;

refuse:
    return 1;
}

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */
void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *p = Actor_Get(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    SceneActor_MoveActorZeroToTarget(pos);
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor)
{
    struct Actor_02000ec8 *ref;
    u8 *fp;
    u8 flag;
    ref = Actor_Get(0);
    fp = &actor->flatla3;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Actor_Get(first);
    struct Slot02000f10 *b = Actor_Get(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Task_Wait(1);
    }
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Actor_Get(0);

    if (ref->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void SceneActor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Task_Wait(1);
            value = track->value;
            limit = track->limit;
            cnt--;
            if (value <= limit) {
                break;
            }
            continue;
        }
        limit = track->limit;
        break;
    }
    track->state = 0;
    track->value = limit;
    track->mark = (s32) 0x80000000;
}

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_02001068(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void SceneEffect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020042fa();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x02009069;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Math_Cos(v);
        vec.y = 0;
        z = Math_Sin(v);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        Func_02001d32(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

/* Return this overlay's state block. */
s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (gGameState.scene == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}

u8 *SceneData_GetTableB85c(void) { return (u8 *)0x0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (gGameState.scene == (s32)&Value_000000ac) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_02004402(tbl);
    return tbl;
}

void SceneState_RunRect73x38Step(void)
{
    Event_Begin();
    {
        s32 width = 9;
        s32 height = 38;

        Map_CopyCellAttributes(73, 38, 5, 5, width, height);
    }
    StagedActor_AdvancePair();
    Func_020036da();
    Event_End();
}

void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor_02000dc8 *actor = Actor_Get(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(point);
}

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */
void FieldScene_RunLayoutAt93By30(void)
{
    Event_Begin();
    {
        s32 width = 29;
        s32 height = 30;

        Map_CopyCellAttributes(93, 30, 6, 5, width, height);
    }
    StagedActor_AdvancePair();
    Func_020037a6();
    Event_End();
}

void FieldScene_RunStepWith6(void)
{
    Func_02004502(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(0);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Event_Begin();
    Map_CopyCellAttributes(89, 49, 3, 2, lead, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, lead, 51);
    StagedActor_AdvancePair();
    Func_02003894();
    Event_End();
}

void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Actor_Get(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (SceneActor_MoveActorZeroToTarget(target)!= 0) {
        SceneState_ApplyTwoRectsAndRunThree();
    }
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses.
 */
void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    GameFlag_Set(0x200);
    if (GameFlag_IsSet(0x201) != 0) {
        Func_0200457c(14)[98] = 0;
        Func_02004588(14)[89] &= (u8)0xf7;
    } else {
        Func_02004598(14)[98] = 1;
        flags = Actor_Get(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    GameFlag_Set(0x201);
    if (GameFlag_IsSet(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        p = Actor_Get(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void SceneState_ApplyFlag970(void)
{
    GameFlag_Set(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor_02001510 *actor = Actor_Get(0);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        SceneActor_ApplyPointLeftOfActorZero();
    }
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct Actor_02000cc0 *actor = Actor_Get(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        SceneActor_PassActorZeroOffsetPoint();
    }
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Event_Begin();
    actor = (struct FieldActor *)Value1(Engine_ActorGet, 18);
    if ((actor->x.fixed >> 20) == 46) {
        Event_Wait(30);
        rec2 = OverlayObject_CreateAndInitialize(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        Actor_Get(18)->motion_flags = 0;
        Audio_PlayCue(185);
        for (i = 0; i < 16; i++) {
            Task_Wait(3);
            Actor_Get(18)->y.fixed -= 0x10000;
            rec7 = Value0(Engine_RandomNext);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Value0(Engine_RandomNext);
            Func_020020de(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Event_Wait(30);
        Actor_Get(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(18, 3);
        Engine_ObjectDispatchRelease(rec2);
        Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Audio_PlayCue(188);
        Map_CopyCellsTo(58, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Event_Wait(20);
        Audio_PlayCue(188);
        Map_CopyCellsTo(59, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        MapRender_WaitForValues();
        Event_Wait(10);
        GameFlag_Set(0x971);
    }
    Event_End();
}

void FieldScene_RunFourCallSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunSupplementalSequenceTwo();
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Actor_SetAnimation(20, 1);
    Actor_SetChildValue(20, 0);
    Actor_SetAnimation(20, 2);
    flags = Func_02004836(20) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x200);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Func_02004842(a0);
    actor = (struct FieldActor *)Value1(Engine_ActorGet, 19);
    if ((actor->x.fixed >> 20) == 48) {
        if (GameFlag_IsSet(0x202) != 0) {
            Event_Wait(30);
            rec2 = OverlayObject_CreateAndInitialize(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            Actor_Get(19)->motion_flags = 0;
            Audio_PlayCue(185);
            for (i = 0; i < 16; i++) {
                Task_Wait(3);
                Actor_Get(19)->y.fixed -= 0x10000;
                rec7 = Value0(Engine_RandomNext);
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Value0(Engine_RandomNext);
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                Func_020022dc(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
            Event_Wait(30);
            Actor_Get(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Actor_SetAnimation(19, 3);
            Engine_ObjectDispatchRelease(rec2);
            Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
            Actor_SetPosition(21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Audio_PlayCue(188);
            Map_CopyCellsTo(58, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(20);
            Audio_PlayCue(188);
            Map_CopyCellsTo(59, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            MapRender_WaitForValues();
            Event_Wait(10);
            GameFlag_Set(0x972);
        }
    }
    Event_End();
}

void FieldScene_RunFourStepSequenceA(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    Func_0200304c();
}

void FieldScene_SetActor19TableB3B8(void)
{
    Engine_ActorEnableActionCallback(19, 0x0200B3B8);
}

void SceneState_SetValue202ThenCall(void)
{
    GameFlag_Set(0x202);
    Func_02003072();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Actor_SetAnimation(21, 1);
    Actor_SetChildValue(21, 0);
    Actor_SetAnimation(21, 2);
    flags = Func_02004a62(21) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    if (GameFlag_IsSet(0x982) != 0 || GameFlag_IsSet(0x983) != 0) {
        Message_ShowCentered(0x268A, 1);
    } else {
        Message_ShowCentered(0x2689, 1);
    }
    Event_End();
}

void FieldScene_RunTwoStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    Event_End();
}

void FieldScene_RunFourStepSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    FieldScene_RunTwoStepSequence();
}

void SceneActor_InstallSlotNineHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *owner;

    Actor_EnableActionCallback(8, 0x0200B3B8);
    GameFlag_Set(0x203);
    owner = Actor_Get(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Func_02008ec8. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *desc;
    s32 col;
    s32 row;

    Event_Begin();
    Actor_SetSpritePriority(9, 1);
    Actor_SetAnimation(9, 1);
    Actor_SetChildValue(9, 0);
    Actor_SetAnimation(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    GameFlag_Set(0x204);

    col = Func_02004b60(9)->col;
    row = Func_02004b68(9)->row >> 20;
    Map_CopyCellAttributes(26, 8, 1, 1, col >> 20, row);

    desc = Actor_Get(9);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    desc = Actor_Get(8);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    Func_02004b84_b(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Actor_Get(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Func_02004d56(slot)->x >> 20) != (Func_02004d5e(subject)->x >> 20)) continue;
        if ((Func_02004d6e(slot)->z >> 20) != (Func_02004d76(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Func_02004d86(slot)->y + 0x100000) continue;

        best = Func_02004d98(slot)->y + 0x100000;
        *(u16 *)((u8 *)Actor_Get(subject) + 100) = (u16)slot;
    }

    Actor_SetSpeed(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Actor_Get(subject);
        Slot_02001c2c *from = Actor_Get(subject);
        s32 z = Func_02004dda(subject)->z;

        Func_02004d4e(target, from->x, best, z);
    }

    Actor_WaitForMove(subject);
    Audio_PlayCue(188);
    Func_02002ecc(subject);
    Event_Wait(30);
}

void FieldScene_RunMiddleSequence(void)
{
    struct FieldActor saved;
    s32 i;
    s32 j;
    s32 found;

    Event_Begin();
    for (i = 0; i <= 2; i++) {
        if (((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->sprite->priority == 3
            && GameFlag_IsSet(i + 0x200) == 0) {
            Actor_Get(i + 12);
            Func_02002d82();
            Actor_SetPosition(i + 12, 0, 0);
            GameFlag_Set(i + 0x200);
            break;
        }
        if ((((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->z.fixed >> 20) == 9
            && GameFlag_IsSet(i + 0x200) == 0) {
            *(s32 *)(Value1_02001d04(Engine_ActorGet, i + 12) + 20) = 0;
            ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->velocity_y = 0;
            *(s32 *)(Func_02004f26(i + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(i + 12))->motion_flags = 0;
            *(u16 *)(Func_02004f3c(i + 12) + 100) = 0;
            found = i;
            for (j = 0; j < i; j++) {
                if (GameFlag_IsSet(0x200 + j) == 0) {
                    saved.x.fixed = ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->x.fixed;
                    saved.y.fixed = ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->y.fixed;
                    saved.z.fixed = ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(i + 12))->x.fixed =
                        ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, j + 12))->x.fixed;
                    ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->y.fixed =
                        ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, j + 12))->y.fixed;
                    ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, i + 12))->z.fixed =
                        ((struct FieldActor *)Value1_02001d04(Engine_ActorGet, j + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->x.fixed = saved.x.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->y.fixed = saved.y.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->z.fixed = saved.z.fixed;
                    found = j;
                    break;
                }
            }
            *(s32 *)(Func_02004f6a(found + 12) + 20) = 0;
            ((struct FieldActor *)Actor_Get(found + 12))->velocity_y = 0;
            *(s32 *)(Func_02004f7e(found + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(found + 12))->motion_flags = 0;
            *(u16 *)(Func_02004f96(found + 12) + 100) = 0;
            Value2(Engine_CameraSetSpeed, 0x30000, 0x6000);
            ((struct FieldActor *)Func_0200502c())->motion_flags = 0;
            Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
            Camera_WaitForMove();
            SceneActor_LandOnHighestPlatform(found + 12);
            if ((((struct FieldActor *)Value1_02001d04(Engine_ActorGet, found + 12))->x.fixed >> 20) == 8) {
                (*(s16 *)(Func_02004fe2(10) + 100))++;
                (*(s16 *)(Func_02004ff0(11) + 100))--;
            } else {
                (*(s16 *)(Func_02004ffe(10) + 100))--;
                (*(s16 *)(Func_0200500c(11) + 100))++;
            }
            ((struct FieldActor *)Actor_Get(found + 12))->update = (void (*)(union FieldObject *))0x2009a99;
            Func_020039d4(40);
            ((struct FieldActor *)Actor_Get(found + 12))->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            GameFlag_Set(0x200 + found);
            break;
        }
    }
    Event_End();
}

void FieldScene_RunThreeStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
}

void SceneState_SetSlot17And18Selectors(void)
{
    Event_Begin();

    if ((Func_02005086(17)->w8 >> 20) == 45) {
        GameFlag_Set(0x974);
    } else {
        GameFlag_Clear(0x974);
    }

    if ((Func_020050a2(18)->w8 >> 20) == 46) {
        GameFlag_Set(0x975);
    } else {
        GameFlag_Clear(0x975);
    }

    Func_020044dc();
    Event_End();
}

void FieldScene_RunFourCallSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Event_Begin();
    StagedActor_AdvancePair();
    SceneState_SetSlot17And18Selectors();
    Event_End();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void FieldScene_RunLateSequenceHead(void)
{
    Map_CopyCellAttributes(72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Map_CopyCellAttributes(113, 43, 1, 1, 49, 43); /* main:080091c0 */
    MapObject_SetPosition(100, 0, 0);
    MapObject_SetPosition(101, 0, 0);
    Actor_SetPosition(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    Actor_SetPosition(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void FieldScene_RunLateSequenceSecond(void)
{
    Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
    Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
    MapObject_SetPosition(100, -1, -1);
    MapObject_SetPosition(101, -1, -1);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
}

void FieldScene_RunScene3c4SequenceA(s32 a0)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Func_0200519e(a0);
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
    record = Value1(Engine_ActorGet, 19);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Engine_ActorGet, 19) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Value1(Engine_ActorGet, 20);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Engine_ActorGet, 20) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Value1(Engine_ActorGet, 21);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Engine_ActorGet, 21) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Value1(Engine_ActorGet, 22);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Engine_ActorGet, 22) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Value1(Engine_ActorGet, 23);
    p5 = *(s32 *)(record + 8);
    q = *(s32 *)(Value1(Engine_ActorGet, 23) + 16);
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Value1(Engine_ActorGet, 19);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Engine_ActorGet, 19);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Value1(Engine_ActorGet, 20);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Engine_ActorGet, 20);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Engine_ActorGet, 21);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Engine_ActorGet, 21);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Engine_ActorGet, 22);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Engine_ActorGet, 22);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Engine_ActorGet, 23);
    if ((*(s32 *)(record + 8) >> 20) == 24) {
        record = Value1(Engine_ActorGet, 23);
        if ((*(s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (GameFlag_IsSet(0x984) != 0) {
            Event_End();
            goto L_020022e4;
        }
        Event_Wait(20);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
        Camera_WaitForMove();
        Event_Wait(30);
        GameFlag_Set(0x984);
        Audio_PlayCue(158);
        Map_AnimateCells(0x200b3ec, 32, 46);
        Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
        Event_Wait(40);
    } else {
        if (GameFlag_IsSet(0x984) != 0) {
            Event_Wait(20);
            Camera_SetSpeed(0xcccc, 0x1999);
            Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            GameFlag_Clear(0x984);
            Audio_PlayCue(159);
            Map_AnimateCells(0x200b40c, 32, 46);
            Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
            Event_Wait(40);
        }
    }
    Event_End();
    L_020022e4:;
}

void FieldScene_RunLayoutAt83By45(void)
{
    Event_Begin();
    {
        s32 width = 19;
        s32 height = 45;

        Map_CopyCellAttributes(83, 45, 11, 8, width, height);
    }
    StagedActor_AdvancePair();
    Func_020043c6();
    Event_End();
}

void SceneState_SetValue268bInScene(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(0x268B, 1);
    Event_End();
}

void FieldScene_RunScriptedStep953(void)
{
    Event_Begin();
    Message_ShowCentered(0x953, 1);
    Event_End();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_000000ac) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Map_CopyCellAttributes(73, 38, 5, 5, p5, p6);
    }
    SceneState_SwapSlotPairByRank(9, 8);

    {
        s32 col = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Map_CopyCellAttributes(93, 30, 6, 5, k5, k6);
    }
    SceneState_SwapSlotPairByRank(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }
}

void FieldScene_RunScene3c4_02002480(void)
{
    s32 record;
    s32 p5;

    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    *(u8 *)(Func_020055ba(14) + 34) = 1;
    record = Value1_02002480(Engine_ActorGet, 12);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Engine_ActorGet, 12);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Value1_02002480(Engine_ActorGet, 13);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Engine_ActorGet, 13);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Value1_02002480(Engine_ActorGet, 14);
    p5 = *(s32 *)(record + 8);
    record = Value1_02002480(Engine_ActorGet, 14);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
}
