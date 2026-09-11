#include "types.h"
#include "scene.h"
#include "spawn_configured_effect.h"
#include "spawn_configured_effect_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"

/* overlays/scene/actor/map_init/map_init.c */
/* overlays/scene/actor/map_init/actor_depth_effect.c */
struct Actor { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };

struct Actor *Actor_unk10_4();

s32 Actor_SetFlagBitByRelativeDepth(struct Actor *actor)
{
    struct Actor *ref;
    u8 *fp;
    u8 flag;
    ref = Actor_unk10_4(0);
    fp = &actor->flags23;
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

/* overlays/scene/actor/map_init/actor_position.c */
struct Actor {
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
typedef struct Slot {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot;

struct Actor *Actor_unk11_4(s32);

struct Actor *Actor_unk12_4(s32);

Record *Actor_unk13_4(s32);
Record *Actor_unk14_4(s32);
Record *Actor_unk15_4(s32);
Record *Actor_unk16_4(s32);
struct Actor *Actor_unk17_4(s32);

struct Actor *Actor_unk18_4(s32);

Slot *Actor_unk19_4();
Slot *Actor_unk20_4();
Slot *Actor_unk21_4();
Slot *Actor_unk22_4();
Slot *Actor_unk23_4();
Slot *Actor_unk24_4();
Slot *Actor_unk25_4();

Slot *Actor_unk26_4();
Slot *Actor_unk27_4();
Slot *Actor_unk28_4();

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */

void Actor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor *p = Actor_unk11_4(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    Actor_unk5_2(pos);
}

void Actor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor *actor = Actor_unk12_4(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    Actor_unk6_2(pos);
}

s32 Actor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Actor_unk13_4(0);

    if (ref->w12 > (s32)0xffd00000
        && (Actor_unk14_4(8)->w16 >> 20) == 10) {
        record->w8 = Actor_unk15_4(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Actor_unk16_4(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void Actor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor *actor = Actor_unk17_4(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Actor_unk7_2(point);
}

void Actor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor *actor = Actor_unk18_4(0);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    Actor_unk8_2(pos);
}

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void Actor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Actor_unk19_4(slot)->x >> 20) != (Actor_unk20_4(subject)->x >> 20)) continue;
        if ((Actor_unk21_4(slot)->z >> 20) != (Actor_unk22_4(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Actor_unk23_4(slot)->y + 0x100000) continue;

        best = Actor_unk24_4(slot)->y + 0x100000;
        *(u16 *)((u8 *)Actor_unk25_4(subject) + 100) = (u16)slot;
    }

    Actor_unk29_4(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot *target = Actor_unk26_4(subject);
        Slot *from = Actor_unk27_4(subject);
        s32 z = Actor_unk28_4(subject)->z;

        Actor_unk30_4(target, from->x, best, z);
    }

    Actor_unk31_4(subject);
    Actor_unk32_4(188);
    Actor_unk33_4(subject);
    Actor_unk34_4(30);
}

/* overlays/scene/actor/map_init/actor_presentation.c */

u8 *Actor_unk35_4(s32);
u8 *Actor_unk36_4(s32);
u8 *Actor_unk37_4(s32);
u8 *Actor_unk38_4(s32);

u8 *Actor_unk39_4(s32);

void Actor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    Actor_unk49(0x201);
    if (Actor_unk50(0x200) != 0) {
        Actor_unk35_4(14)[98] = 0;
        Actor_unk36_4(14)[89] &= (u8)0xf7;
    } else {
        Actor_unk37_4(14)[98] = 1;
        p = Actor_unk38_4(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void ActorDraw_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Actor_Apply(20, 1);
    Actor_Apply2(20, 0);
    Actor_Apply3(20, 2);
    flags = Actor_unk39_4(20) + 35;
    *flags &= 0xFD;
    Actor_unk9_2(0x200);
}

/* overlays/scene/actor/map_init/actor_search.c */
struct Actor02001424 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Actor02001424 *Actor_unk40_4(s32);

void Actor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Actor_unk40_4(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (Actor_unk51(target)!= 0) {
        Actor_unk41_4();
    }
}

/* overlays/scene/actor/map_init/conditional_scene_setup.c */
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Actor *Actor_unk42_4(s32);

void State_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor *actor = Actor_unk42_4(0);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        Actor_unk43_4();
    }
}

/* overlays/scene/actor/map_init/configurable_effect_spawn.c */

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
}

/* overlays/scene/actor/map_init/dialogue_layout.c */
typedef struct Slot2 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot2;

Slot2 *Actor_unk44_4();
Slot2 *Actor_unk45_4();

Slot2 *Actor_unk46_4();
Slot2 *Actor_unk47_4();

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

void Scene_RunLayoutAt93By30(void)
{
    Actor_unk48_4();
    {
        s32 width = 29;
        s32 height = 30;

        Actor_unk21_5(93, 30, 6, 5, width, height);
    }
    Actor_unk49_4();
    Actor_unk50_4();
    Actor_unk51_4();
}

void Scene_RunLayoutAt83By45(void)
{
    Actor_unk52_4();
    {
        s32 width = 19;
        s32 height = 45;

        Actor_unk22_5(83, 45, 11, 8, width, height);
    }
    Actor_unk53_4();
    Actor_unk54_4();
    Actor_unk55_4();
}

void Scene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Actor_unk56_4(93, 30, 6, 5, k5, k6);
    }
    Actor_unk57_4(11, 10);

    {
        s32 col20 = Actor_unk44_4(10)->column >> 20;
        row = Actor_unk45_4(10)->row >> 20;
        Actor_unk58_4(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Actor_unk46_4(11)->column >> 20;
        row = Actor_unk47_4(11)->row >> 20;
        Actor_unk59_4(2, 36, 1, 1, col20, row);
    }
}

/* overlays/scene/actor/map_init/effect.c */
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

u8 *Actor_unk60_4(s32 kind, s32 x, s32 y, s32 z);

struct SceneObject *Actor_unk61_4(void);

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */

u8 *OvObj_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Actor_unk60_4(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Actor_Apply4(obj, 0);
        Actor_Apply5(obj, 14);
        Actor_Apply6(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

void Effect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Actor_unk61_4();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x02009069;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Actor_unk52(v);
        vec.y = 0;
        z = Actor_unk53(v);
        x = vec.x;
        vec.z = z;
        x = x + Actor_Apply7(x, 3);
        vec.x = x;
        Actor_unk23_5(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

/* overlays/scene/actor/map_init/effect_motion_variant_a.c */
/*
 * Per-frame integrator for an effect record -- resource_3c4. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
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

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void advance_effect_motion(struct Effect *effect)
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

    effect->velocity[0] = velocity_x - Actor_unk54(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

/* overlays/scene/actor/map_init/move_actor_zero_to_target.c */
/*
 * Actor movement for overlay resource_3c4.  Declarations are left without
 * prototypes because Actor_unk63_4 and Actor_unk64_4 are each called twice
 * with different argument shapes.
 */

typedef struct Actor {
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
} Actor;

typedef struct Target {
    s32 x;                 /* +0, 12.20 fixed point */
    s32 pad4;
    s32 z;                 /* +8, 12.20 fixed point */
} Target;

Actor *Actor_unk65_4();

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 Actor_MoveActorZeroToTarget(const Target *target)
{
    Actor *actor = Actor_unk65_4(0);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Actor_unk55(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Actor_unk56(actor, probe) == 1) {
        goto refuse;
    }
    if (Actor_unk57(actor, target) != 0) {
        goto refuse;
    }

    Actor_unk66_4();
    Actor_unk67_4(actor, 6);
    Actor_unk68_4(6);
    Actor_unk69_4(152);
    Actor_unk70_4(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Actor_unk71_4(actor, 0);
    Actor_unk72_4(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Actor_unk73_4(actor, 6);
    Actor_unk74_4(actor, 1);
    Actor_unk58(6);

    actor->flags = saved;
    Actor_unk75_4();
    return 0;

refuse:
    return 1;
}

/* overlays/scene/actor/map_init/overlay_object.c */
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

u8 *Actor_unk76_4(s32 kind, s32 x, s32 y, s32 z);

struct Actor *Actor_unk77_4(s32);

/*
 * Spawn an object and prepare its owner record and flags.  The three callees
 * are declared at their in-overlay entry points, not at veneers.  Each
 * read-modify-write takes its mask from a local that is initialised first and
 * ANDed with the loaded byte afterwards, which is what puts the mask in r3 and
 * the byte in r2.  The +0x23 mask stays byte-wide as 0xfe rather than ~1,
 * which would widen it to a negated 2.
 */

u8 *OvObj_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Actor_unk76_4(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Actor_Apply8(obj, 0);
        Actor_Apply9(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

void OvObj_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

s32 OvObj_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Actor_unk77_4(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}

/* overlays/scene/actor/map_init/scene_data.c */
extern s16 gCell[];
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];

/* Return this overlay's state block. */

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_000000ac) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (gCell[224] == (s32)&Value_000000ad) {
        return (s32)gOv4;
    }
    return 0;
}

u8 *SceneData_GetTableB85c(void) { return (u8 *)0x0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (gCell[224] == (s32)&Value_000000ac) {
        tbl = gOv5;
    } else {
        tbl = gOv6;
    }
    Actor_unk10_2(tbl);
    return tbl;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_000000ac) {
        return (s32)gOv7;
    }
    return (s32)gOv8;
}

/* overlays/scene/actor/map_init/scene_dialogue.c */

void Dialogue_RunFlag982Or983Dialogue(void)
{
    Actor_unk78_4();
    Actor_Apply10(0, 1);
    if (Actor_unk59(0x982) != 0 || Actor_unk60(0x983) != 0) {
        Actor_Apply11(0x268A, 1);
    } else {
        Actor_Apply12(0x2689, 1);
    }
    Actor_unk79_4();
}

/* overlays/scene/actor/map_init/scene_primary_script.c */

extern u8 gWork[];

u8 *Actor_unk80_4();

u8 *Actor_unk81_4();

u8 *Actor_unk82_4();

u8 *Actor_unk83_4();

void Scene_RunStepWith6(void)
{
    Actor_unk13_3(0, 6, 0);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Actor_unk84_4();
    record = Actor_Check(18);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 46) {
    } else {
        Actor_unk85_4(30);
        rec2 = Actor_unk61(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        *(u8 *)(Actor_unk62(18) + 85) = 0;
        Actor_unk86_4(185);
        for (i = 0; i < 16; i++) {
            Actor_unk87_4(3);
            record = Actor_unk80_4(18);
            *(volatile s32 *)((s32)record + 12) += -0x10000;
            rec7 = Actor_Run();
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Actor_unk2_4();
            Actor_unk88_4(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Actor_SetRect(51, 8, 1, 1, 49, 8);
        Actor_unk89_4(30);
        {
            u8 *record = Actor_unk81_4(18);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Actor_unk90_4(18, 3);
        Actor_unk91_4(rec2);
        Actor_unk2_5(45, 4, 1, 1, 46, 8);
        Actor_Place(20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Actor_unk92_4(188);
        Actor_unk3_5(58, 8, 49, 8, v5, v6);
        Actor_unk2_3(0, 0x50000, 0x10000);
        Actor_unk3_3(-1, -1, 0xe666);
        Actor_unk93_4(20);
        Actor_unk94_4(188);
        Actor_unk4_5(59, 8, 49, 8, v5, v6);
        Actor_unk4_3(0, 0x50000, 0x10000);
        Actor_unk5_3(-1, -1, 0xe666);
        Actor_unk95_4();
        Actor_unk96_4(10);
        Actor_Do(0x971);
    }
    Actor_unk97_4();
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Actor_unk98_4(a0);
    record = Actor_unk2(19);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 48) {
    } else {
        if (Actor_unk3(0x202) == 0) {
        } else {
            Actor_unk99_4(30);
            rec2 = Actor_unk63(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            *(u8 *)(Actor_unk64(19) + 85) = 0;
            Actor_unk100_4(185);
            for (i = 0; i < 16; i++) {
                Actor_unk101_4(3);
                record = Actor_unk82_4(19);
                *(volatile s32 *)((s32)record + 12) += -0x10000;
                rec7 = Actor_unk3_4();
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Actor_unk4_4();
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                Actor_unk102_4(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Actor_unk5_5(51, 8, 1, 1, 45, 14);
            Actor_unk103_4(30);
            {
                u8 *record = Actor_unk83_4(19);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
            Actor_unk104_4(19, 3);
            Actor_unk105_4(rec2);
            Actor_unk6_5(45, 4, 1, 1, 48, 14);
            Actor_unk6_3(21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Actor_unk106_4(188);
            Actor_unk7_5(58, 8, 45, 14, v5, v6);
            Actor_unk7_3(0, 0x50000, 0x10000);
            Actor_unk8_3(-1, -1, 0xe666);
            Actor_unk107_4(20);
            Actor_unk108_4(188);
            Actor_unk8_5(59, 8, 45, 14, v5, v6);
            Actor_unk9_3(0, 0x50000, 0x10000);
            Actor_unk10_3(-1, -1, 0xe666);
            Actor_unk109_4();
            Actor_unk110_4(10);
            Actor_unk2_2(0x972);
        }
    }
    Actor_unk111_4();
}

void Scene_SetActor19TableB3B8(void)
{
    Actor_Apply13(19, 0x0200B3B8);
}

void Scene_RunMiddleSequence(void)
{
    s32 saved[28];
    s32 frame;
    s32 outer;
    s32 base;
    s32 zero;
    s32 found;
    s32 inner;
    s32 slot;
    s32 rec;
    s32 tmp;
    s32 dst;
    s32 pa;
    s32 pb;
    s32 v0;
    s32 va;
    s32 v4;
    s32 vb;
    s32 v3;

    Actor_unk112_4();
    outer = 0;
    frame = (s32)saved;
    zero = outer;
    base = 12;
    L_02001d20:;
    rec = Actor_unk4(base);
    if ((12 & *(u8 *)(*(volatile s32 *)(rec + 80) + 9)) == 12) {
        slot = outer + 0x200;
        if (Actor_unk5(slot) == 0) {
            Actor_unk113_4(base);
            Actor_unk114_4();
            Actor_unk115_4(base, 0, 0);
            Actor_unk116_4(slot);
            goto L_02001f40;
        }
    }
    goto L_02001de4;
    L_02001d5e:;
    rec = Actor_unk6(base);
    *(s32 *)(frame + 8) = *(volatile s32 *)(rec + 8);
    rec = Actor_unk7(base);
    *(s32 *)(frame + 12) = *(volatile s32 *)(rec + 12);
    rec = Actor_unk8(base);
    *(s32 *)(frame + 16) = *(volatile s32 *)(rec + 16);
    dst = Actor_unk65(base);
    pa = inner + 12;
    rec = Actor_unk9(pa);
    *(volatile s32 *)(dst + 8) = *(volatile s32 *)(rec + 8);
    dst = Actor_unk10(base);
    rec = Actor_unk11(pa);
    *(volatile s32 *)(dst + 12) = *(volatile s32 *)(rec + 12);
    dst = Actor_unk12(base);
    rec = Actor_unk13(pa);
    *(volatile s32 *)(dst + 16) = *(volatile s32 *)(rec + 16);
    rec = Actor_unk66(pa);
    *(volatile s32 *)(rec + 8) = *(s32 *)(frame + 8);
    rec = Actor_unk67(pa);
    *(volatile s32 *)(rec + 12) = *(s32 *)(frame + 12);
    found = inner;
    rec = Actor_unk68(pa);
    *(volatile s32 *)(rec + 16) = *(s32 *)(frame + 16);
    goto L_02001e56;
    L_02001de4:;
    rec = Actor_unk14(base);
    if ((*(volatile s32 *)(rec + 16) >> 20) != 9) {
        goto L_02001f36;
    }
    tmp = Actor_unk15(outer + 0x200);
    if (tmp != 0) {
        goto L_02001f36;
    }
    rec = Actor_unk16(base);
    *(volatile s32 *)(rec + 20) = tmp;
    rec = Actor_unk17(base);
    *(volatile s32 *)(rec + 40) = tmp;
    rec = Actor_unk69(base);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Actor_unk70(base) + 85) = tmp;
    *(volatile u16 *)(Actor_unk71(base) + 100) = tmp;
    found = outer;
    for (inner = 0; inner < outer; inner++) {
        if (Actor_unk18(0x200 + inner) == 0) {
            goto L_02001d5e;
        }
    }
    L_02001e56:;
    pb = found + 12;
    rec = Actor_unk72(pb);
    *(volatile s32 *)(rec + 20) = zero;
    rec = Actor_unk73(pb);
    *(volatile s32 *)(rec + 40) = zero;
    rec = Actor_unk74(pb);
    *(volatile s32 *)(rec + 60) = -0x80000000;
    *(u8 *)(Actor_unk75(pb) + 85) = zero;
    *(volatile u16 *)(Actor_unk76(pb) + 100) = zero;
    Actor_unk19(0x30000, 0x6000);
    *(u8 *)(Actor_unk77() + 85) = zero;
    Actor_unk5_4(0xa80000, 0x80000, 0xb80000, 1);
    Actor_unk117_4();
    Actor_unk118_4(pb);
    rec = Actor_unk20(pb);
    if ((*(volatile s32 *)(rec + 8) >> 20) == 8) {
        vb = (Actor_unk78(10) + 100);
        *(volatile u16 *)(vb) = (*(volatile u16 *)(vb) + 1);
        v0 = (Actor_unk79(11) + 100);
        v3 = (*(volatile u16 *)(v0)- 1);
    } else {
        va = (Actor_unk80(10) + 100);
        v4 = (*(volatile u16 *)(va)- 1);
        *(volatile u16 *)(va) = v4;
        v0 = (Actor_unk81(11) + 100);
        v3 = (*(volatile u16 *)(v0) + 1);
    }
    *(volatile u16 *)(v0) = v3;
    rec = Actor_unk82(pb);
    *(volatile s32 *)(rec + 108) = 0x2009a99;
    Actor_unk119_4(40);
    *(u8 *)(Actor_unk83(pb) + 35) |= 2;
    Actor_unk120_4(0x200 + found);
    goto L_02001f40;
    L_02001f36:;
    outer = outer + 1;
    base = base + 1;
    if (outer <= 2) {
        goto L_02001d20;
    }
    L_02001f40:;
    Actor_unk121_4();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void Scene_RunLateSequenceHead(void)
{
    Actor_unk9_5(72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Actor_unk10_5(113, 43, 1, 1, 49, 43); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(100, 0, 0);
    EffectRuntime_SetCurrentPosition_2(101, 0, 0);
    Motion_SetHPosTerrain_1(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    Motion_SetHPosTerrain_2(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void Scene_RunLateSequenceSecond(void)
{
    Actor_unk11_5(8, 113, 1, 1, 8, 49);
    Actor_unk12_5(49, 107, 1, 1, 49, 43);
    EffectRuntime_SetCurrentPosition_1(100, -1, -1);
    EffectRuntime_SetCurrentPosition_2(101, -1, -1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(15, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(16, 0, 0);
}

void Scene_RunScene3c4SequenceA(s32 a0)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Actor_unk122_4(a0);
    Actor_unk13_5(83, 45, 11, 8, 19, 45);
    record = Actor_unk21(19);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Actor_unk22(19) + 16);
    q >>= 20;
    p5 >>= 20;
    Actor_unk123_4(20, 56, 1, 1, p5, q);
    record = Actor_unk23(20);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Actor_unk24(20) + 16);
    q >>= 20;
    p5 >>= 20;
    Actor_unk124_4(20, 56, 1, 1, p5, q);
    record = Actor_unk25(21);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Actor_unk26(21) + 16);
    q >>= 20;
    p5 >>= 20;
    Actor_unk125_4(20, 56, 1, 1, p5, q);
    record = Actor_unk27(22);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Actor_unk28(22) + 16);
    q >>= 20;
    p5 >>= 20;
    Actor_unk126_4(20, 56, 1, 1, p5, q);
    record = Actor_unk29(23);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Actor_unk30(23) + 16);
    q >>= 20;
    p5 >>= 20;
    Actor_unk127_4(20, 56, 1, 1, p5, q);
    record = Actor_unk31(19);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Actor_unk32(19);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Actor_unk33(20);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Actor_unk34(20);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Actor_unk35(21);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Actor_unk36(21);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Actor_unk37(22);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Actor_unk38(22);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Actor_unk39(23);
    if ((*(volatile s32 *)(record + 8) >> 20) == 24) {
        record = Actor_unk40(23);
        if ((*(volatile s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (Actor_unk41(0x984) != 0) {
            Actor_unk128_4();
            goto L_020022e4;
        }
        Actor_unk129_4(20);
        Actor_unk6_4(0xcccc, 0x1999);
        Actor_unk7_4(0x1d80000, -1, 0x30c0000, 1);
        Actor_unk130_4();
        Actor_unk131_3(30);
        Actor_unk3_2(0x984);
        Actor_unk132_3(158);
        Actor_unk11_3(0x200b3ec, 32, 46);
        Actor_unk14_5(24, 60, 1, 1, 32, 47);
        Actor_unk133_3(40);
    } else {
        if (Actor_unk42(0x984) != 0) {
            Actor_unk134_3(20);
            Actor_unk8_4(0xcccc, 0x1999);
            Actor_unk9_4(0x1d80000, -1, 0x30c0000, 1);
            Actor_unk135_3();
            Actor_unk136_3(30);
            Actor_unk4_2(0x984);
            Actor_unk137_3(159);
            Actor_unk12_3(0x200b40c, 32, 46);
            Actor_unk15_5(31, 47, 1, 1, 32, 47);
            Actor_unk138_3(40);
        }
    }
    Actor_unk139_3();
    L_020022e4:;
}

void Scene_RunScriptedStep953(void)
{
    Actor_unk140_3();
    Actor_Apply14(0x953, 1);
    Actor_unk141_3();
}

void Scene_RunScene3c4(void)
{
    s32 record;
    s32 p5;

    Actor_unk16_5(89, 49, 3, 2, 25, 49);
    Actor_unk17_5(89, 51, 8, 5, 25, 51);
    *(u8 *)(Actor_unk84(14) + 34) = 1;
    record = Actor_unk43(12);
    p5 = *(volatile s32 *)(record + 8);
    record = Actor_unk44(12);
    p5 = p5 >> 20;
    Actor_unk18_5(22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Actor_unk45(13);
    p5 = *(volatile s32 *)(record + 8);
    record = Actor_unk46(13);
    p5 = p5 >> 20;
    Actor_unk19_5(22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Actor_unk47(14);
    p5 = *(volatile s32 *)(record + 8);
    record = Actor_unk48(14);
    p5 = p5 >> 20;
    Actor_unk20_5(22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
}

/* overlays/scene/actor/map_init/scene_setup.c */
typedef struct Slot3 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot3;

typedef struct Slot4 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot4;

u8 *Actor_unk142_3(s32);

u8 *Actor_unk143_3();

Slot3 *Actor_unk144_3();
Slot3 *Actor_unk145_3();

u8 *Actor_unk146_3();
u8 *Actor_unk147_3();
u8 *Actor_unk148_3();

typedef s32(*Handler)(struct Record *record);

Slot4 *Actor_unk149_3();
Slot4 *Actor_unk150_3();

Slot4 *Actor_unk151_3();
Slot4 *Actor_unk152_3();

/*
 * Slot nine setup for overlay resource_3c4.
 */

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Actor_unk153_3 serves two sites with different argument shapes
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

struct Record;

void Scene_RunFourCallSequenceB(void)
{
    Actor_unk154_3();
    Actor_unk155_2();
    Actor_unk156_2();
    Actor_unk157_2();
}

void Scene_RunFourStepSequenceA(void)
{
    Actor_unk158_2();
    Actor_unk159_2();
    Actor_unk160_2();
    Actor_unk161_2();
}

void Scene_RunTwoStepSequence(void)
{
    Actor_unk162_2();
    Actor_unk163_2();
}

void Scene_RunFourStepSequenceB(void)
{
    Actor_unk164_2();
    OvObj_SetYAboveLinkedActor();
    Actor_unk165_2();
    Actor_unk166_2();
}

void Actor_InstallSlotNineHandler(void)
{
    u8 *owner;

    Actor_Apply15(8, 0x0200B3B8);
    Actor_unk11_2(0x203);
    owner = Actor_unk142_3(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Actor_unk85. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void Actor_SetupSlotNineAndInstallHandler(void)
{
    u8 *desc;
    s32 col;
    s32 row;

    Actor_unk167_2();
    Actor_unk168_2(9, 1);
    Actor_unk169_2(9, 1);
    Actor_unk170_2(9, 0);
    Actor_unk171_2(9, 2);

    {
        u8 *flag = Actor_unk143_3(9) + 35;
        *flag &= (u8)0xfd;
    }

    Actor_unk172_2(0x204);

    col = Actor_unk144_3(9)->col;
    row = Actor_unk145_3(9)->row >> 20;
    Actor_unk173_2(26, 8, 1, 1, col >> 20, row);

    desc = Actor_unk146_3(9);
    *(Handler *)(desc + 108) = Actor_unk85;

    desc = Actor_unk148_3(8);
    *(Handler *)(desc + 108) = Actor_unk85;

    Actor_unk147_3(desc);
}

void Scene_RunThreeStepSequence(void)
{
    Actor_unk174_2();
    Actor_unk175_2();
    Actor_unk176_2();
}

void Scene_RunFourCallSequence(void)
{
    Actor_unk177_2();
    Actor_unk178_2();
    Actor_unk179_2();
    Actor_unk180_2();
}

void Scene_PlaceAndPinSlots8And9(void)
{
    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Actor_unk181_2(73, 38, 5, 5, p5, p6);
    }
    Actor_unk182_2(9, 8);

    {
        s32 col = Actor_unk149_3(8)->column >> 20;
        row = Actor_unk150_3(8)->row >> 20;
        Actor_unk183_2(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Actor_unk151_3(9)->column >> 20;
        row = Actor_unk152_3(9)->row >> 20;
        Actor_unk184_2(2, 36, 1, 1, col, row);
    }
}

/* overlays/scene/actor/map_init/staged_actor.c */

s32 StagedActor_FindClearPosition(s32 *a)
{
}

/* overlays/scene/actor/map_init/staged_actor_navigation.c */
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

extern s32 ActorSearchStep[];
extern s32 gOv9[];

Ent *Actor_unk185_2(Desc *, Ent *);

s32 Actor_CheckAheadTileAndStop(Ent *obj)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *hit;

    idx = obj->unk6 >> 12;
    m = ActorSearchStep[idx];
    d.unk0 = obj->unk8 + (m & 0xffff0000);
    d.unk4 = obj->unkC;
    m = m << 16;
    d.unk8 = obj->unk10 + m;
    hit = Actor_unk185_2(&d, obj);
    if (hit != 0) {
        u32 i = 0;
        s32 v = *hit->unk50->unk28;
        s32 *p = gOv9;

        do {
            if (v == *p++) {
                goto done;
            }
            i++;
        } while (i <= 5);
        obj->unk24 = 0;
        obj->unk2C = 0;
        obj->unk38 = 0x80000000;
        obj->unk40 = 0x80000000;
    }
    m = ActorSearchStep[idx];
    d.unk0 = obj->unk8 + (m & 0xffff0000);
    d.unk4 = obj->unkC;
    m = m << 16;
    d.unk8 = obj->unk10 + m;
    if (Actor_EntOp(obj, &d) > 0) {
        obj->unk24 = 0;
        obj->unk2C = 0;
        obj->unk38 = 0x80000000;
        obj->unk40 = 0x80000000;
    }
done:
    return 0;
}

/* overlays/scene/actor/map_init/staged_actor_pair.c */

extern u32 ActorSearchStep[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void StagedActor_AdvanceActorPair(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *block;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = FindNextStagedActor(dst, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    block = FindBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dst[0], dst[1], dst[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}

/* overlays/scene/actor/map_init/state_update.c */
struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot5 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot5;

struct Slot02000f10 *Actor_unk186_2(s32);
struct Slot02000f10 *Actor_unk187_2(s32);

u8 *Actor_unk188_2(s32);
u8 *Actor_unk189_2(s32);
u8 *Actor_unk190_2(s32);
u8 *Actor_unk191_2(s32);

struct Actor *Actor_unk192_2(s32);

u8 *Actor_unk193_2(s32);

Slot5 *Actor_unk194_2(s32 slot);
Slot5 *Actor_unk195_2(s32 slot);

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

s32 State_ApplyArgMode0AndReturnZero(s32 no)
{
    Actor_Apply16(no, 0);
    return 0;
}

void State_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Actor_unk186_2(first);
    struct Slot02000f10 *b = Actor_unk187_2(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Actor_unk12_2(1);
    }
}

void State_RunRect73x38Step(void)
{
    Actor_unk196_2();
    {
        s32 width = 9;
        s32 height = 38;

        Actor_unk24_5(73, 38, 5, 5, width, height);
    }
    Actor_unk197_2();
    Actor_unk198_2();
    Actor_unk199_2();
}

void State_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Actor_unk200_2();
    Actor_unk25_5(89, 49, 3, 2, lead, 49);
    Actor_unk26_5(89, 51, 8, 5, lead, 51);
    Actor_unk201_2();
    Actor_unk202_2();
    Actor_unk203_2();
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
void Actor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    Actor_unk13_2(0x200);
    if (Actor_unk86(0x201) != 0) {
        Actor_unk188_2(14)[98] = 0;
        Actor_unk189_2(14)[89] &= (u8)0xf7;
    } else {
        Actor_unk190_2(14)[98] = 1;
        flags = Actor_unk191_2(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void State_ApplyFlag970(void)
{
    Actor_unk14_2(0x970);
}

void State_RunUnlessActorZeroAt30_52(void)
{
    struct Actor *actor = Actor_unk192_2(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        Actor_unk204_2();
    }
}

void State_SetValue202ThenCall(void)
{
    Actor_unk15_2(0x202);
    Actor_unk205_2();
}

void Actor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Actor_Apply17(21, 1);
    Actor_Apply18(21, 0);
    Actor_Apply19(21, 2);
    flags = Actor_unk193_2(21) + 35;
    *flags &= 0xFD;
    Actor_unk16_2(0x201);
}

void State_SetSlot17And18Selectors(void)
{
    Actor_unk206_2();

    if ((Actor_unk194_2(17)->w8 >> 20) == 45) {
        Actor_unk17_2(0x974);
    } else {
        Actor_unk18_2(0x974);
    }

    if ((Actor_unk195_2(18)->w8 >> 20) == 46) {
        Actor_unk19_2(0x975);
    } else {
        Actor_unk20_2(0x975);
    }

    Actor_unk207_2();
    Actor_unk208_2();
}

void State_SetValue268bInScene(void)
{
    Actor_unk209_2();
    Actor_Apply20(0, 1);
    Actor_Apply21(0x268B, 1);
    Actor_unk210_2();
}

/* overlays/actor_move_and_redraw.c */
/* overlays/scene/actor/map_init/move_and_redraw.c */
void Actor_Run(
    StagedActorMovementRequest request)
{
}

/* overlays/shared/copy_mode_to_owner.c */
/* Copy the low two mode bits into the object's owner record. */

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

    owner->mode = mode;
}

/* overlays/scene/actor/map_init/wait_value_below_limit.c */
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

void Actor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Actor_Do(1);
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
