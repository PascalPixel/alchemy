#include "types.h"
#include "scene.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor {
    u8 reserved00[91];
    u8 accepted;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor2 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct Presentation {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor3 {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation *presentation;
};

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork {
    u8 reserved000[370];
    u16 actor18_marker;
};

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork2 {
    u8 reserved000[472];
    u16 branch_counter;
};

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor2 {
    u8 reserved00[91];
    u8 accepted;
};

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor3 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern u8 SceneMessage_ActorFourteenBase;
extern const unsigned char gOv[];
extern const unsigned char gOv2[];
extern const unsigned char gOv3[];
extern const u8 gOv4[];
extern void Actor_unk5_4(s32 dialogue);                 /* Event_SetValue1d8 */
extern struct Actor *Actor_unk7_4(s32 actor);  /* Scene_GetRecord */
extern void Actor_unk8_4(s32 actor);                    /* ActorDraw_RunActorModeOneThenZero */
extern struct Actor *Actor_unk9_4(s32 actor);  /* Scene_GetRecord */
extern const u8 gOv5[];
extern void Actor_unk10_4(s32 dialogue);                 /* Event_SetValue1d8 */
extern struct Actor2 *Actor_unk11_4(s32 actor);  /* Scene_GetRecord */
extern void Actor_unk12_4(s32 actor);                    /* ActorDraw_RunActorModeOneThenZero */
extern struct Actor2 *Actor_unk13_4(s32 actor);  /* Scene_GetRecord */
extern u8 gVal[];
extern u8 gVal2[];
extern const u8 gOv6[];
extern const u8 gOv7[];
extern const u8 gOv8[];
extern s16 State_Table[];

void *AcquireOverlayObject(s32, s32, s32, s32);

struct SceneActor *Actor_unk14_4(s32);

struct SceneActor *Actor_unk15_4(s32);

struct SceneActor *Actor_unk16_4(s32);

typedef s32(*IwramIntegerSquareRoot)(s32);

u8 *Scene_GetPartnerActor();

u8 *Scene_GetPlayerActor();

u8 *Actor_unk17_4(int);

struct SceneActor *Actor_unk18_4(s32);

struct SceneActor *Actor_unk19_4(s32);

struct SceneActor *Actor_unk20_4(s32);

struct SceneActor *Actor_unk21_4(s32);

struct Actor3 *Actor_unk22_4(s32 actor);

u8 *Actor_unk23_4();

s32 SceneFlag_Check(s32 flag);
void SceneFlag_Set(s32 flag);
void SceneMap_UpdateRect(s32, s32, s32, s32, s32, s32);
void SceneMap_CopyRect(s32, s32, s32, s32, s32, s32);
void State_SetMode(s32 mode);
u8 *Actor_Find(s32 actor);
void Actor_Place(s32 actor, s32 x, s32 z);

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

/* The four declarations name this overlay's import veneers. */

/* Complete table-address getter through its sole return and literal pool. */

/* Complete overlay-local table-address getter through its literal pool. */

/* Complete in-image scene-descriptor getter through its literal pool. */

/*
 * Every call is named at its decoded bl site: the two Scene_GetRecord reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * ActorDraw_RunActorModeOneThenZero calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */

/* Complete scene-variant six actor-zero setup wrapper through its pool. */

/* Complete actor-mode scene wrapper through its return and alignment. */

/* Complete paired scene-cell presentation update. */

/* Complete alternate paired scene-cell presentation update. */

/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Actor_unk24_4/Actor_unk25_4,
 * but a direct bl cannot encode that displacement, and only the RAM-resident
 * sites reproduce the reference bytes.  The reference also materializes both
 * stacked constants before either store, which named locals preserve.
 */

/* Complete actor-8 flag-setting dialogue scene through its two-word pool. */

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/* Complete actor-8 timed flag/dialogue scene through its two-word pool. */

/* Complete actor-12 timed two-flag dialogue scene through its pool. */

/* Complete scene-variant five actor-zero setup wrapper through its pool. */

/* Complete scene-variant seven actor-zero setup wrapper through its pool. */

/* Complete scene-variant ten actor-zero setup wrapper through its pool. */

/* Complete paired scene-resource enable wrapper through its two-word pool. */

static __inline__ s32 Scene_QueryFlag(s32 (*func)(s32), s32 flag)
{
    extern u8 gWork[];

    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    extern u8 gWork[];

    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    extern u8 gWork[];

    func(a, b, c);
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    extern u8 *gWork;

    Actor_unk10_3(actor, horizontal, vertical);
}

/*
 * Complete actor-21 mode-four reset wrapper.
 *
 * Call symbol resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(`push {lr} / movs r0,#21 / movs r1,#0 /
 * movs r2,#4 / bl sub / pop {r0} / bx r0`): the per-site overlay
 * veneer is sub, not a main-image address guessed from elsewhere.
 */

static __inline__ void PlaceActor(void (*place)(s32, s32, s32),
                                 s32 actor, s32 x, s32 z)
{
    void *OvObj_CreateConfiguredObjectB(s32, s32, s32, s32);

    place(actor, x, z);
}

static __inline__ void UpdateRect(void (*update)(s32, s32, s32, s32, s32, s32),
                                 s32 x, s32 z, s32 width, s32 height,
                                 s32 sourceX, s32 sourceZ)
{
    void *OvObj_CreateConfiguredObjectB(s32, s32, s32, s32);

    update(x, z, width, height, sourceX, sourceZ);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OvObj_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObjectB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void Effect_UpdateMotion(union MotionWork *work)
{
    extern u8 gWork[];

    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
    extern u8 gWork[];

#include "configured_effect_spawn_body.inc"
}

s32 CalculateFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 Actor_UpdateProximity(struct SceneActor *actor, struct SceneActor *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Actor_Apply(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Actor_Apply2(target->z - actor->z,
                                      *targetPos - *actorPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Actor_Apply3(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Actor_Apply4(actor, 2);
    }
    return result;
}

s32 UpdateActorProximity(u8 *actor)
{
    extern u8 *gWork;

    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(actor + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Scene_GetPartnerActor(15);
    } else {
        partner = Scene_GetPartnerActor(14);
    }
    if (Actor_CheckProximity(actor, partner, 32, 0) != 0) {
        return 0;
    }

    player = Scene_GetPlayerActor(0);

    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Actor_UpdateProximity(actor, player, range, force);
    return 0;
}

const void *SceneData_GetTable9450(void)
{
    return gOv;
}

/* Complete zero-return leaf; no calls and no argument read. */
int SceneData_ReturnZero(void)
{
    return 0;
}

const void *SceneData_GetTable95A0(void)
{
    return gOv2;
}

const void *SceneData_GetTable95D8(void)
{
    return gOv3;
}

void ActorDraw_RunActorModeOneThenZero(s32 actor)
{
    extern struct SceneWork *gWork;

    Actor_unk26_4();
    Actor_Apply5(actor, 1);
    Actor_Apply6(actor, 0);
    Actor_unk27_4();
}

void Dialogue_RunActor8FlagScene(void)
{
    extern struct SceneWork2 *gWork;

    Actor_unk28_4();
    Actor_unk11_3(8, 0, 2);
    Actor_unk7_2(0x305);
    Actor_unk8_2(0x1cab);
    Actor_Apply7(8, 0);
    Actor_unk29_4();
}

void Dialogue_RunActor11Line(void)
{
    extern struct SceneWork2 *gWork;

    /*
 * Complete actor-11 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 753-765, still unadopted at the
 * time of writing): `bl sub2`, `bl sub3`, then
 * `bl .L_020009d6` -- the third call targets a local branch label at
 * 0x020009d6, not the guessed 0x020004b4.
 */

    Actor_unk9_2(0x1cae);
    Actor_unk12_3(11, 0, 2);
    Actor_unk10_2(11);
}

void Dialogue_RunActor12TwoFlagScene(void)
{
    extern struct SceneWork2 *gWork;

    /*
 * Complete actor-12 two-flag dialogue scene through its three-word pool.
 *
 * Call symbols are per-site, not per-import: the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 766-783, still unadopted) spells
 * every call as `bl sub_02000xxx`, and the two calls that both ultimately
 * reach GameFlag_Set go through TWO DIFFERENT overlay-local veneers,
 * sub4 and sub5 -- not the same symbol twice.
 */

    Actor_unk30_4();
    Actor_unk13_3(12, 0, 2);
    Actor_unk11_2(0x306);
    Actor_unk12_2(0x868);
    Actor_unk13_2(0x1caf);
    Actor_Apply8(12, 0);
    Actor_unk31_4();
}

void Dialogue_ShowLine1CB0ForActor13(void)
{
    extern struct SceneWork2 *gWork;

    /*
 * Complete actor-13 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 757-769, still unadopted at the
 * time of writing): `bl sub6`, `bl sub7`, `bl sub8`.
 */

    Actor_unk14_2(0x1cb0);
    Actor_unk14_3(13, 0, 2);
    Actor_unk15_2(13);
}

void Dialogue_RunActorFourteenFlagDialogue(void)
{
    extern u8 gWork[];

    struct SceneActor *actor = Actor_unk14_4(14);
    s16 facing = (s16)actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Actor_unk32_4();
    text = (s32)&SceneMessage_ActorFourteenBase;
    Actor_unk16_2(text);
    Actor_Apply9(14, 0);
    Actor_unk15_3(14, 0, 2);
    if (Scene_QueryFlag(Actor_unk6, 0x300) == 0) {
        Scene_Call3(Actor_unk16_3, 14, 256, 60);
        Actor_unk17_3(14, 0, 10);
        Actor_unk18_3(14, 0, 10);
        Scene_SetFlag(Actor_unk17_2, 0x300);
    }
    Actor_unk18_2(text + 2);
    Actor_unk19_3(14, 0, 10);
    actor->facing = facing;
    Actor_unk19_2(1);
    Actor_unk33_4();
    {
        s32 shown = 1;
        actor->state_flags = shown;
    }
    Actor_unk20_2(0x307);
}

void Dialogue_RunActorFifteenFacingPreservedDialogue(void)
{
    extern u8 gWork[];

    struct Actor3 *actor;
    s16 facing0;

    actor = Actor_unk22_4(15);
    facing0 = (s16)actor->facing;
    actor->state_flags |= 2;
    Actor_unk34_4();
    Actor_unk21_2(0x1cb4);
    Actor_Apply10(15, 0);
    Actor_unk20_3(15, 0, 2);
    Actor_unk21_3(15, 0, 10);
    actor->facing = (u16)facing0;
    Actor_unk22_2(1);
    Actor_unk35_4();
    actor->state_flags = 0;
}

void Dialogue_RunActor16CountedDialogue(void)
{
    extern struct SceneWork2 *gWork;

    Actor_unk36_4();
    Actor_unk23_2(0x1cb5);
    Actor_unk22_3(16, 0, 2);
    Actor_Apply11(16, 0);
    if (Actor_Apply12(0, 0) != 0) {
        gWork->branch_counter += 1;
    }
    Actor_Apply13(16, 0);
    Actor_unk24_2(0x308);
    Actor_unk37_4();
}

void Dialogue_RunActorEightTimedDialogue(void)
{
    extern struct SceneWork2 *gWork;

    Actor_unk38_4();
    Actor_Apply14(8, 1);
    Actor_unk25_2(20);
    Actor_unk23_3(8, 0, 20);
    Actor_unk26_2(0x305);
    Actor_unk27_2(0x1cab);
    Actor_unk24_3(8, 0, 20);
    Actor_unk39_4();
}

void Dialogue_RunActor11AcceptanceDialogue(void)
{
    extern struct SceneWork2 *gWork;

    Actor_unk10_4(0x1cbd);
    Actor_unk11_4(11)->accepted = 1;
    Actor_unk12_4(11);
    Actor_unk13_4(11)->accepted = 0;
}

void Dialogue_RunActor12TimedTwoFlagScene(void)
{
    extern struct SceneWork2 *gWork;

    Actor_unk40_4();
    Actor_Apply15(12, 1);
    Actor_unk28_2(20);
    Actor_unk25_3(12, 0, 20);
    Actor_unk29_2(0x306);
    Actor_unk30_2(0x868);
    Actor_unk31_2(0x1caf);
    Actor_unk26_3(12, 0, 20);
    Actor_unk41_4();
}

void ActorDraw_RunActor13AcceptanceDialogue(void)
{
    extern struct SceneWork *gWork;

    Actor_unk5_4(0x1cbf);
    Actor_unk7_4(13)->accepted = 1;
    Actor_unk8_4(13);
    Actor_unk9_4(13)->accepted = 0;
}

void Scene_RunSupplementalSequenceOne(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;

    {
        u16 *flags = (u16 *)(Actor_unk7(14) + 100);
        u16 bits = (u16)(s32)gVal;

        *flags |= bits;
    }
    Actor_unk42_4();
    if (Actor_Check(0x307) != 0) {
        Actor_unk43_4((s32)gVal2);
        Actor_unk44_4(14);
    } else {
        Actor_unk45_4();
        Actor_Do(0x307);
    }
    Actor_unk46_4();
    {
        u8 *record = Actor_unk23_4(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}

void Dialogue_RunActorFifteenDialogue(void)
{
    extern u8 gWork[];

    {
        struct SceneActor *actor = Actor_unk15_4(15);
        actor->state_flags |= 2;
    }
    Actor_unk47_4();
    Actor_unk32_2(0x1cc1);
    Actor_unk33_2(15);
    Actor_unk48_4();
    {
        s32 clear = 0;
        struct SceneActor *actor = Actor_unk16_4(15);
        actor->state_flags = clear;
    }
}

void Scene_RunScene385SequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = Actor_unk2(0x308);
    if (rec7 == 0) {
        Actor_unk49_4();
        *(u8 *)(Actor_unk8(16) + 91) = 1;
        Actor_unk50_4(16, 1);
        Actor_Run(16, 1);
        Actor_unk51_4(20);
        Actor_unk2_2(0x1cb5);
        Actor_unk2_3(16, 0, 2);
        Actor_unk3(16, 0);
        if (Actor_unk4(0, 0) != 0) {
            bump_step(1);
        }
        Actor_unk52_4(16, 0);
        *(u8 *)(Actor_unk9(16) + 91) = rec7;
        Actor_unk53_4(16, 2);
        Actor_unk2_4();
        Actor_unk3_2(0x308);
    } else {
        Actor_unk4_2(0x1cc2);
        *(u8 *)(Actor_unk10(16) + 91) = 1;
        Actor_unk54_4(16);
        v5 = 0;
        *(u8 *)(Actor_unk11(16) + 91) = v5;
    }
}

void move_actor_and_wait(int actor, int x, int z, int field40)
{
    extern u8 *gWork;

    u8 *record = Actor_unk17_4(actor); int frames;
    Actor_unk27_3(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Actor_Apply16(record, 0);
    Actor_unk28_3(actor, x, z); Actor_unk29_3(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Actor_unk34_2(1); if (*(s16 *)(record + 42) == 0) break; }
    Actor_Apply17(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void Scene_RunScene385(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk56_4();
    Actor_unk57_4(100);
    Actor_unk58_4(40);
    if (Actor_unk5(0x867) == 0) {
        Actor_unk3_4(21, 0x102);
        Actor_unk59_4(21, 4, 0);
        Actor_unk60_4(12);
        Actor_unk61_4(21, 4, 0);
        Actor_unk62_4(20);
        Actor_unk4_4(21, 0x188, 104, 0x70000);
        Actor_unk63_4(20);
        Actor_unk3_3(21, 0x198, 104);
        Actor_unk4_3(21, 0x198, 120);
        Actor_unk5_2(0x867);
    }
    Actor_unk64_4();
}

void PlaceActorAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
    extern u8 *gWork;

    s32 zero = 0;

    SetScale(zero, 0x8000, 0x4000);
    Actor_unk30_3(zero, x, y);
    *(s32 *)(gWork + 456) = 16;
    Actor_unk35_2(delay);
}

void Scene_SetupDescriptor9740(void)
{
    Actor_unk36_2(158);
    Actor_unk31_3(gOv6, 56, 19);
    Actor_unk32_3(408, 320, 5);
}

void ActorDraw_SetupActorEighteenAt312_304(void)
{
    extern struct SceneWork *gWork;

    Actor_unk37_2(158);
    Actor_unk33_3(gOv5, 50, 18);
    Actor_unk34_3(312, 304, 6);
}

void Scene_SetupWithDescriptor976C(void)
{
    Actor_unk38_2(158);
    Actor_unk35_3(gOv7, 44, 17);
    Actor_unk36_3(216, 288, 7);
}

void ActorDraw_SetupActorZeroForSceneEight(void)
{
    extern struct SceneWork *gWork;

    struct SceneActor2 *actor = Actor_unk18_4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Actor_unk39_2(158);
    Actor_unk37_3((const void *)0x02009782, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Actor_SetRect(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Actor_unk38_3(376, 224, 8);
}

void ActorDraw_SetupActorZeroForSceneNine(void)
{
    extern struct SceneWork *gWork;

    struct SceneActor2 *actor = Actor_unk19_4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Actor_unk40_2(158);
    Actor_unk39_3((const void *)0x02009798, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Actor_unk2_5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Actor_unk40_3(296, 176, 9);
}

void Scene_SetupWithDescriptor97AE(void)
{
    Actor_unk41_2(158);
    Actor_unk41_3(gOv8, 38, 6);
    Actor_unk42_3(120, 144, 10);
}

void ActorDraw_SetupActorSceneCells(void)
{
    extern struct SceneWork *gWork;

    struct SceneActor3 *actor = Actor_unk21_4(0);
    struct Presentation *record = actor->presentation;
    u8 flags;

    Actor_unk42_2(188);
    Actor_unk3_5(42, 33, 34, 16, 2, 2);
    Actor_unk4_5(42, 35, 36, 16, 2, 2);
    Actor_unk43_2(4);
    Actor_unk5_5(40, 33, 34, 16, 2, 2);
    Actor_unk6_5(40, 35, 36, 16, 2, 2);
    Actor_unk44_2(4);
    {
        s32 mode = 3;
        s32 value = 16;
        Actor_unk7_5(33, 21, 2, 2, mode, value);
    }
    actor->state_23 &= ~1;
    flags = record->flags;
    flags |= 12;
    record->flags = flags;
    Actor_unk43_3(64, 272, 11);
}

void ActorDraw_SetupActorZeroForSceneTwelve(void)
{
    extern struct SceneWork *gWork;

    struct SceneActor3 *actor = Actor_unk20_4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Actor_unk45_2(158);
    Actor_unk44_3((const void *)0x020097c4, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Actor_unk8_5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Actor_unk45_3(72, 160, 12);
}

void State_SetFlag200AndConfigureRegion55_26(void)
{
    Actor_unk46_2(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Actor_unk9_5(55, 26, 4, 2, a, b);
    }
}

void ActorDraw_SetFlag200AndSceneCell23(void)
{
    extern struct SceneWork *gWork;

    Actor_unk47_2(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Actor_unk10_5(23, 23, 4, 2, first_value, second_value);
    }
}

void Scene_SetActor21Values0And4(void)
{
    extern u8 gWork[];

    Actor_unk46_3(21, 0, 4);
}

void Scene_RunScene385(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk65_4(231);
    Actor_unk66_4();
    Actor_unk67_4(10);
    Actor_unk68_4(18, 2);
    Actor_unk5_3(18, 0xcccc, 0x6666);
    Actor_unk6_3(18, 216, 0x198);
    Actor_unk69_4(10);
    Actor_unk7_3(18, 0x4000, 20);
    Actor_unk70_4(18, 6, 0);
    Actor_unk71_4(30);
    Actor_unk72_4(18, 6, 0);
    Actor_unk73_4(30);
    Actor_unk74_4(18, 6, 0);
    Actor_unk75_4(30);
    Actor_unk8_3(18, 216, 0x188);
    Actor_unk76_4(10);
    Actor_unk9_3(18, 0x4000, 20);
    Actor_unk6_2(0x858);
    Actor_unk77_4();
}

void ActorDraw_SetPairedSceneCells(void)
{
    extern struct SceneWork *gWork;

    s32 v1 = 13;
    s32 v2 = 25;

    Actor_unk11_5(41, 43, 1, 1, v1, v2);
    Actor_unk12_5(40, 42, 12, 22, 3, 3);
}

void ActorDraw_SetAlternatePairedSceneCells(void)
{
    extern struct SceneWork *gWork;

    s32 v1 = 13;
    s32 v2 = 25;

    Actor_unk13_5(37, 43, 1, 1, v1, v2);
    Actor_unk14_5(36, 42, 12, 22, 3, 3);
}

void Scene_RunActorEighteenDialogue(void)
{
    extern struct SceneWork *gWork;

    Actor_unk78_4();
    Actor_unk48_2(0x1342);
    Actor_Apply18(18, 0);
    Actor_unk47_3(18, 0, 0);
    Actor_unk49_2(2);
    Actor_Apply19(18, 0);
    Actor_Apply20(18, 1);

    if (Actor_unk12(231) != -1 && Actor_unk13(0x858) == 0) {
        gWork->actor18_marker = 1;
    }

    Actor_unk79_4();
}

void State_SetFlag947AndValue29dc(void)
{
    Actor_unk80_4();
    Actor_Apply21(0x947, 1);
    Actor_Apply22(0x29dc, 1);
    Actor_unk81_4();
}

const u8 *SceneData_GetTable97dc(void)
{
    return gOv4;
}

s32 Setup_InitializeActorsAndFlags(void)
{
    void *OvObj_CreateConfiguredObjectB(s32, s32, s32, s32);

    u8 *actor;
    s16 *scene;
    s32 mode;

    if (SceneFlag_Check(0x200))
        UpdateRect(SceneMap_UpdateRect, 55, 26, 4, 2, 23, 26);
    OvObj_CreateConfiguredObjectB(0x800000, 0, 0x1a40000, 223);
    SceneMap_CopyRect(45, 41, 8, 45, 3, 3);
    State_SetMode(1);
    actor = Actor_Find(14);
    *(u32 *)(actor + 108) = 0x02008401;
    {
        u8 *actor = Actor_Find(14);
        s32 mode = 1;
        *(u16 *)(actor + 100) = mode;
    }
    mode = 0;
    actor = Actor_Find(15);
    *(u32 *)(actor + 108) = 0x02008401;
    *(u16 *)(Actor_Find(15) + 100) = mode;
    if (SceneFlag_Check(0x858))
        PlaceActor(Actor_Place, 18, 0xd80000, 0x1880000);
    if (State_Table[225] <= 2 && !SceneFlag_Check(52) && !SceneFlag_Check(0x109))
        SceneFlag_Set(0x867);
    if (SceneFlag_Check(0x867) && !SceneFlag_Check(52))
        PlaceActor(Actor_Place, 21, 0x1980000, 0x780000);
    scene = State_Table;
    if (scene[225] == 11)
        SceneFlag_Set(0x12f);
    if (scene[225] == 13)
        SceneFlag_Set(0x120);
    return 0;
}
