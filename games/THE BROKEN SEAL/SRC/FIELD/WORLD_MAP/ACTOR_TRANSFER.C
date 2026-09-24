#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

enum ActorTransferMessage {
    MSG_AFTER_BRINGING_DJINNI_INTO_YOUR = 0xc66,
    MSG_NEXT_ILL_SHOW_HOW_CAN = 0xc67,
    MSG_WE_CANT_STAY_ANOTHER_MINUTE = 0x1215,
    MSG_NOW_USE_ON_SHIP = 0x2642,
    MSG_ROBIN_WHERE_GOING_SAID_USE = 0x2643,
    MSG_WRECKAGE_SHIP_SCUTTLED_OFF_COAST = 0x264c,
    MSG_SUKURETA_HOW_LONG_WILL_ISLAND = 0x2913
};


#define RatioHistory_RecordAndScheduleCallback_1(a0, a1) Call2(Func_020077c6, a0, a1)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR 8

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct StorySelection {
    u8 reserved000[500];
    s32 actor_id;
};

struct StorySelectionActor {
    u8 reserved00[6];
    u16 presentation;
};

struct StoryProgressWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryCompletionWork {
    u8 reserved000[386];
    s16 scene_value;
};

/*
 * Per-frame step of a thirty-two frame rise -- resource_371.  The halfword
 * counter at +0x64 is incremented in place and compared as a signed sixteen
 * bit value; past 31 the effect is handed off and nothing else happens.  The
 * anchor pointer at +0x68 is loaded before the frame test and must stay
 * there.  The angle is the frame shifted left by ten, and the callee that
 * turns it into a 16.16 amplitude is identified only by that use.
 */
struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;  /* 0x68 */
};

/*
 * The mirror of the rise: the same counter at +0x64 and anchor at +0x68,
 * with the second amplitude store negated, the depth offset subtracted
 * rather than added, and a fixed depth bias of 0x100000.  Its magnitude
 * callee, like the one above, is identified only by use -- one angle in, one
 * 16.16 magnitude out.
 */
struct StoryVerticalEffectActor_02004004 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor_02004004 *anchor;  /* 0x68 */
};

extern u8 Data_0200811d[];
extern u8 Data_020081c5[];
extern u8 Value_00000847;
extern u8 Value_000002f1;
extern u8 Data_0200d27c[];
extern u8 Data_0200da2c[];
extern u8 Data_0200e3f4[];
extern u8 Data_0200db84[];
extern u8 Data_0200e04c[];
extern u8 Data_0200e154[];
extern u8 Data_0200e1e4[];
extern u8 Data_0200e25c[];
extern u8 Data_0200e28c[];
extern u8 Data_0200e2ec[];
extern u8 Data_0200e394[];
extern u8 Data_0200e3c4[];
extern u8 Value_0000085a;
extern s32 Data_0200e79c;
extern u8 Data_00000000[];
extern s32 Data_02000240_t[][1];
extern u8 Data_0200cd98[];
extern u8 Data_0200cac4[];
extern u8 Data_0200ce78[];
extern u8 Data_0200caf4[];
extern s32 Data_0200e718[];
extern struct StorySharedState *Data_03001f30;

void Func_02004484(s32, s32);
void Func_020044d2();
void Func_0200451a();
void Func_02006d0c(void);
void Func_02006d3a(void);
void Func_02007328(void *, s32);
s32 Func_0200811c(s32, s32);
void Func_02005ecc_a();
void Func_02006070();
void Func_0200607a();
void Func_020060a2();
void Func_020060a2_a();
void Func_020060ae();
void Func_020060f4();
void Func_0200611c();
void Func_02006cbc();
void Func_02006cd8();
void Func_02006f44();
void Func_020070ec();
void Func_02007164();
void Func_020071a0();
void Func_020071a0_a();
void Func_020071d4();
void Func_02007202();
s32 Func_02007260();
void Func_020072a8();
void Func_020073fa();
void Func_02007484();
void Func_0200749a();
void Func_020077c6();
s32 Func_02007778();
void Func_02007a22();
void Func_02007aac();
s32 Func_020078d6();
void Func_02007986();
void Func_02007a38();
s32 Func_02007af8();
void Func_02004ef8();
void Func_0200503e();
void Func_020050ac();
void Func_020050ca();
void Func_0200514a_a();
void Func_0200529a();
void Func_020052d0_a();
void Func_02005304_a();
void Func_020053b0_a();
void Func_02005478();
void Func_02005658();
void Func_0200567a();
void Func_02005736();
void Func_020057be();
s32 Func_0200793e(s32 state);
void Func_02007974(s32 state, s32 value);
void Func_02007c22(s32 actor, s32 mode);
void Func_02007c34(s32 actor, s32 mode);
void Func_02007ab2(s32 flag, s32 value);
void Func_02007af0(u32 address, u32 value);
void Func_02007b0a(u32 address, u32 value);
void Func_02007b34(u32 address, u32 value);
void Func_02007f98(u8 *);
s32 Func_020080e0(s32, s32);
void Func_02007fd4(u8 *);

/*
 * Story-progress triggers -- resource_371.  Each fires one event once the
 * progress word has reached nine tenths of the level word; each seventy-two
 * byte owner includes its three pool words.
 */

/* Actor callbacks for the story transfer scene. */

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address; the declarations stay old-style because the interfaces
 * are unknown.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 StoryScene_ComputeOpposingSlotDelta(void);

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
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

static __inline__ void Call4_020017fc(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001888(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001938(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_020019e8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{

    return f();
}

static __inline__ void Call1_02001a98(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call4_02001a98(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

static __inline__ void Call1_02001b5c(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call4_02001b5c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ void Call2_020032f0(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4_02000c28(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/*
 * The progress word is Data_02000240[284] read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.  Each Func_ name labels the call word of one call site rather
 * than a runtime address, so the same service appears under several names,
 * and the declarations stay old-style because the interfaces are unknown and
 * argument counts differ between call sites.
 */
void StoryProgress_TriggerEvent0808(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *workspace = Data_03001ebc;
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        if ((u32)Random_Next() < 0x8000) {
            Func_02004484(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        Func_020044d2(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        Func_0200451a(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /*
     * Arm order decides the branch sense: the fall-through is the increment
     * and the taken branch is the call.  Swapping the arms inverts the test.
     */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Engine_ObjectDispatchRelease(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{
    extern u32 Data_03001e40;

    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((Data_03001e40 & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Object_Create(0x11d, x, y, z);
    }
    Audio_PlayCue(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Actor_SetSpriteFlags(spawned_actor, 0);
        Object_SetAnimation(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)Data_0200811d;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    u8 *actor_flags;
    s32 fixed_scale;

    if (GameFlag_IsSet(0x30) != 0) {
        return 0;
    }
    if (GameFlag_IsSet(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)Data_020081c5;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}

u8 *SceneData_GetTableD27C(void)
{
    return Data_0200d27c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableDA2C(void)
{
    return Data_0200da2c;
}

s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    StoryActor_ApplyMapRotation();
    if (GameFlag_IsSet((s32)&Value_00000847) != 0) {
        Object_SetAnimation(actor, 2);
    }
    return 1;
}

/* Turns the actor's sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotation(struct FieldActor *actor)
{
    struct MapRenderWork *work = Data_03001e70;
    struct FieldSprite *sprite = actor->sprite;

    sprite->rotation = work->rotation;
    sprite->flags = 0;
    return 1;
}

/* Sets the actor's first collision flag and turns its sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor)
{
    struct MapRenderWork *work = Data_03001e70;
    struct FieldSprite *sprite = actor->sprite;

    actor->collision_flags |= 1;
    sprite->rotation = work->rotation;
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    s32 zero;
    Actor_SetSpriteFlags(actor, 0);
    Object_SetPartPalettes(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (GameFlag_IsSet(0x8A0) != 0) {
        GameFlag_Set((s32)&Value_000002f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}

/*
 * The popped register is r1, so r0 survives the return and is the result.
 * The owner includes its alignment halfword and its one pool word, the
 * address of Data_03001e40 -- a live status word, not overlay image data.
 * The exclusive or against the bit just tested clears bit 0 only.  value is
 * a byte, so value >> 8 is zero and both branches do the same clear and
 * store; that shape is deliberate and decides the register allocation.
 */
s32 StoryActor_ClearActiveFlag(u8 *actor)
{
    extern u32 Data_03001e40;

    u8 *active_flags = actor + 0x54;
    u8 value = *active_flags;
    u32 active_bit = 1 + (value >> 8);

    if ((active_bit & value) != 0 && (Data_03001e40 & active_bit) != 0) {
        u8 cleared = 1;

        if (active_flags) {
            cleared ^= value;
            *active_flags = (u8)cleared;
        } else {
            cleared ^= value;
            *active_flags = (u8)cleared;
        }
    }
    return 1;
}

/* Each Func_ name identifies one call site's loader-relocated call word rather
 * than a runtime address, so several names here reach the same import. The
 * declarations are old-style because the interfaces are not established. */

/*
 * Selects the placement and spawn table for the current scene. The selector is
 * the signed halfword Data_02000240[225]; only 49 through 80 are covered and
 * everything else takes the default arm, which calls Engine_GameFlagSet before
 * returning. Cases 49 and 64 are conditional and fall through to the default
 * when their test fails. The case arms are in the order the reference uses,
 * not ascending, and that order is what reproduces it.
 */
u8 *StoryScene_SelectPlacementTable(void)
{
    extern s16 Data_02000240[];
    s16 *scene_table = Data_02000240;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (GameFlag_IsSet(0x94f) == 0 && GameFlag_IsSet(0x941) != 0) {
            return Data_0200e154;
        }
        break;
    case 64:
        if (GameFlag_IsSet(0x85a) == 0) {
            return Data_0200e04c;
        }
        break;
    case 65:
    case 70:
        return Data_0200e1e4;
    case 71:
        return Data_0200e28c;
    case 72:
        return Data_0200e394;
    case 73:
        return Data_0200e3c4;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return Data_0200e25c;
    case 80:
        return Data_0200e2ec;
    default:
        break;
    }

    GameFlag_Set(0x235);
    return Data_0200db84;
}

/* Publishes one of two branch values at +0x170 of the scene state, chosen by
 * comparing the other actor's x against the subject's. */
void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Actor_Get(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Actor_Get(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Audio_PlayCue(0x7B);
}

/* The same branch value, chosen on z instead of x. */
void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Actor_Get(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Actor_Get(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Audio_PlayCue(0x7B);
}

void SceneState_SetValues130_6_47(void)
{
    StoryScene_SetBranchValueFromX(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
    StoryScene_SetBranchValueFromZ(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
    StoryScene_SetBranchValueFromZ(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
    StoryScene_SetBranchValueFromZ(0x97, 0x19, 0x36);
}

void FieldScene_RunStep7D3B1E(void)
{

    StoryScene_SetBranchValueFromZ(0x7D, 0x3B, 0x1E);
}

u8 *SceneData_GetTableE3F4(void)
{
    return Data_0200e3f4;
}

enum {
    CONTACT_LAST_ACTOR = 65,
    /* An actor's touch trigger is its id plus this base. */
    CONTACT_TRIGGER_BASE = 100,
    FLAG_CONTACT_PAUSED = 0x163,
    FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 Data_03001f54;

/*
 * Marks each placed actor active while it stands inside a window around the
 * view centre: 160 pixels to either side, 300 pixels toward lower depth and
 * 200 toward higher depth. When an active actor comes within reach of the
 * selected actor, measured as the sum of the axis distances against both
 * scaled radii, its trigger is recorded as touched unless flag 0x104 is set.
 */
void MapActor_UpdateContact(void)
{
    struct FieldActor *leader;
    struct EventWork *work;
    struct FieldActor *actor;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 leader_reach;
    u32 i;

    leader = Actor_Get(gGameState.selected_actor);
    leader_reach = leader->sprite->scale * leader->radius;
    work = gEventWork;
    actor = work->view_center;
    left = actor->x.fixed - PIXELS(160);
    right = actor->x.fixed + PIXELS(160);
    top = actor->z.fixed - PIXELS(300);
    bottom = actor->z.fixed + PIXELS(200);

    for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
        s32 x;
        s32 z;
        s32 dx;
        s32 reach;
        s32 scale;

        actor = Actor_Lookup(i);
        if (actor == NULL) {
            continue;
        }
        x = actor->x.fixed;
        z = actor->z.fixed;
        if (x < left || x > right || z < top || z > bottom) {
            actor->active = 0;
            continue;
        }
        actor->active = 1;
        if (Data_03001f54 != 0 && GameFlag_IsSet(FLAG_CONTACT_PAUSED) != 0) {
            continue;
        }
        scale = actor->sprite->scale;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        reach = leader_reach + actor->radius * scale;
        if (dx + (actor->z.fixed - leader->z.fixed < 0 ? leader->z.fixed - actor->z.fixed
                                                       : actor->z.fixed - leader->z.fixed)
                < reach
            && GameFlag_IsSet(FLAG_CONTACT_BLOCKED) == 0) {
            work->touched_trigger = i + CONTACT_TRIGGER_BASE;
        }
    }
}

void SceneState_ApplyFlag85aBranch(void)
{
    if (GameFlag_IsSet((s32)&Value_0000085a) == 0) {
        Event_RequestExit(101);
    } else {
        Audio_PlayCue(123);
        Event_RequestExit(3);
    }
}

void FieldScene_RunStep74(void)
{

    Event_Begin();
    Event_RequestExit(74);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_02004ef8();
    GameFlag_Set(0x94f);
    Actor_SetPosition(11, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(11, 24, 8);
    Actor_WaitForMove(11);
    Event_Wait(60);
    Actor_SetPosition(12, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(12, 12, 24);
    Event_Wait(30);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Event_Wait(60);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Event_Wait(120);
    Actor_SetPosition(8, 0x16f80000, 0x4b80000);
    Event_Wait(60);
    Actor_SetAnimation(12, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(12);
    Actor_SetPosition(12, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(11, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Actor_SetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetDestinationOffset(8, 56, 8);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 40, 40);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 8, 88);
    Actor_WaitForMove(8);
    Event_CloseScreen();
    Event_RequestExit(108);
    Event_End();
}

void FieldScene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Pointer1(Engine_ActorGet, 15);
    Event_Begin();
    Call2(Func_020050ca, 0x14000, 1);
    Task_Wait(4);
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_020050ac();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_SetPosition(8, 0x16d80000, 0x6280000);
    Task_Wait(1);
    Actor_SetChildValue(8, 15);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetSpeed(11, 0x19999, 0x6666);
    Actor_SetSpeed(12, 0x19999, 0x6666);
    Actor_SetSpeed(13, 0x19999, 0x6666);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, 0x200c9e4);
    Event_Wait(20);
    Actor_EnableActionCallback(11, 0x200cb50);
    Event_Wait(20);
    Actor_EnableActionCallback(12, 0x200cc74);
    Event_Wait(20);
    Call2(Func_0200503e, 13, (s32)Data_0200cd98);
    Audio_PlayCue(0x121);
    record = Pointer1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1704, 0x640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetPosition(8, 0x16d80000, 0x6380000);
    Task_Wait(1);
    Event_SetMessage(MSG_WE_CANT_STAY_ANOTHER_MINUTE);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, 0x200ca78);
    Event_Wait(20);
    Actor_EnableActionCallback(11, 0x200cbe4);
    Event_Wait(20);
    Actor_EnableActionCallback(12, 0x200cd08);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
    Call2(Func_0200514a_a, 13, 0x200ce2c);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Func_0200529a();
    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Func_020052d0_a();
    Actor_SetPosition(9, 0x16080000, 0x6d80000);
    Task_Wait(1);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6f8);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0, 20);
    Actor_StartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetPosition(8, 0x16180000, 0x6f80000);
    Task_Wait(1);
    Actor_SetChildValue(8, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1608, 0x6f8);
    Event_Wait(20);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(8, 0x3000, 60);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 40);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_ShowEmote(9, 0x101, 60);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Func_02005478();
    Audio_PlayCue(107);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_0200155c();
    Audio_PlayCue(0x121);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(14, 0x15a80000, 0x6a80000);
    Task_Wait(1);
    Actor_SetSpeed(14, 0x4ccc, 0x2666);
    Actor_EnableActionCallback(14, 0x200cebc);
    Event_Wait(160);
    *(s32 *)(actor + 72) = 0x1999;
    *(s32 *)(actor + 68) = 0x1999;
    *(s32 *)(actor + 24) = 0x18000;
    *(s32 *)(actor + 28) = 0x18000;
    {
        s32 shown = 0;

        *(u16 *)(actor + 100) = shown;
    }
    *(s32 *)(actor + 12) = 0x400000;
    {
        u8 *target = *(u8 **)(actor + 80);
        s32 shown = 0xf000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_SetSpriteFlags(actor, 0);
    Object_SetAnimation(actor, 2);
    Task_Wait(1);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    Call2(Func_02005304_a, 0x20097a5, 0xc80);
    do {
        Task_Wait(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(14);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(10);
    scale = 192;
    record = Actor_Get(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Pointer1(Engine_ActorGet, 8);
    *(s32 *)(record + 40) = (scale << 11);
    Audio_PlayCue(145);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_02001680();
    FieldScene_RunScene371_02001680();
    Event_Wait(60);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Func_02005658();
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Call1(Func_020053b0_a, 0x20097a5);
    Task_Wait(1);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_Jump(8, 4, 40);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(8, 0xc000, 40);
    Event_ShowMessageAndWait(0x2008, 0, 20);
    Actor_Jump(9, 4, 20);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x8000, 10);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_SetAnimationAndWait(9, 3);
    Actor_WalkToAndWait(8, 0x1618, 0x6f8);
    Actor_SetPosition(8, 0, 0);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6d8);
    Actor_SetPosition(9, 0, 0);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, (s32)Data_0200cac4);
    Actor_EnableActionCallback(11, 0x200cc30);
    Event_Wait(40);
    Actor_EnableActionCallback(12, 0x200cd54);
    Event_Wait(40);
    Call2(Func_0200567a, 13, (s32)Data_0200ce78);
    Func_020057be();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
    Actor_SetPosition(ACTOR_GERALD, 0x17140000, 0x6400000);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    action = (s32)Data_0200caf4;
    Actor_EnableActionCallback(10, action);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x16d80000, -1, 0x6080000, 1);
    Actor_EnableActionCallback(11, action);
    Event_Wait(20);
    Actor_EnableActionCallback(12, action);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_EnableActionCallback(13, action);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Func_02005736(13);
    Audio_PlayCue(0x121);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16f80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 80);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Pointer1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x85a);
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_RunScene371_0200155c(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x160c0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_02001680(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x15ec0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_020017a4(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 15);
    record = Value1(Engine_ActorGet, 14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (GameFlag_IsSet(0x200) == 0) {
            Audio_PlayCue(145);
            Object_SetAnimation(rec7, 3);
            GameFlag_Set(0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void FieldScene_RunScene371_020017fc(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x14a8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(102);
    Event_End();
}

void FieldScene_RunScene371_02001888(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, 0x200d158);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(103);
    Event_End();
}

void FieldScene_RunScene371_02001938(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, 0x200d158);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(104);
    Event_End();
}

void FieldScene_RunScene371_020019e8(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, 0x200d158);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(105);
    Event_End();
}

void FieldScene_RunScene371_02001a98(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Value0(StoryScene_ComputeOpposingSlotDelta) == 11) {
        Engine_ActorEnableActionCallback(8, 0x200d1b8);
    } else {
        Engine_ActorEnableActionCallback(8, 0x200d158);
    }
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(106);
    Event_End();
}

void FieldScene_RunScene371_02001b5c(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x13c8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x93e);
    GameFlag_Clear(0x927);
    Event_RequestExit(107);
    Event_End();
}

void FieldScene_RunScene371_02001c08(void)
{

    u32 i;
    s32 record;

    Func_02006070();
    Call2(Func_020060a2, 0x10000, 6);
    Func_020060ae();
    Func_0200607a();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage(MSG_AFTER_BRINGING_DJINNI_INTO_YOUR);
    Event_ShowMessage(8, 0);
    Event_Wait(30);
    Audio_PlayCue(111);
    Func_02005ecc_a(0, 2);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
    Func_020060f4();
    Actor_Jump(8, 4, 30);
    Event_SetMessage(MSG_NEXT_ILL_SHOW_HOW_CAN);
    Event_ShowMessage(8, 0);
    GameFlag_Clear(0x16f);
    GameFlag_Set(0x171);
    Func_0200611c();
    Event_Wait(30);
    Func_020060a2_a(12, 6);
}

void FieldScene_RunScene371_02002274(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Value1(Engine_ActorGet, 10);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    actor->scale_x = 0x18000;
    actor->scale_y = 0x18000;
    actor->facing = 0x4000;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(10, 0x15680000, 0x8380000);
    Task_Wait(1);
    Audio_PlayCue(141);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x156d, 0x858);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x15b80000, -1, 0x8580000, 1);
    Actor_MoveToAndWait(10, 0x159e, 0x858);
    Actor_MoveToAndWait(10, 0x15a8, 0x86e);
    Actor_MoveToAndWait(10, 0x15e8, 0x878);
    Actor_SetAnimation(10, 1);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x15d80000, 0x8780000);
    Task_Wait(1);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15c8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Audio_PlayCue(141);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x15f8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_MoveToAndWait(10, 0x15f8, 0x838);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(10, 0x15bd, 0x838);
    Actor_MoveToAndWait(10, 0x15b8, 0x853);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_MoveToAndWait(10, 0x1572, 0x858);
    Actor_MoveToAndWait(10, 0x1568, 0x838);
    Actor_SetPosition(10, 0, 0);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Camera_MoveTo(0x15d80000, -1, 0x8580000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15d8, 0x858);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(20);
    Event_End();
}

void FieldScene_RunScene371_0200281c(void)
{
    Event_Begin();
    Actor_FaceActor(55, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage(MSG_NOW_USE_ON_SHIP);
    Value2(Engine_EventShowMessage, Data_0200e79c, 0);
    Actor_FaceDirection(55, 0x3000, 0);
    Event_End();
}

void FieldScene_RunScene371_02002858(void)
{
    Event_Begin();
    Func_02006cbc();
    Event_SetMessage(MSG_ROBIN_WHERE_GOING_SAID_USE);
    Value2(Engine_EventShowMessage, Data_0200e79c, 0);
    Func_02006cd8();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1778, 0xd48);
    Event_End();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{
    extern struct StoryDialogueWork *Data_03001ebc;

    Event_Begin();
    Func_02006d0c();
    Message_ShowCentered(MSG_WRECKAGE_SHIP_SCUTTLED_OFF_COAST, 1);
    if (GameFlag_IsSet(0x234) != 0) {
        Data_03001ebc->story_result = 1;
    }
    Func_02006d3a();
    Event_End();
}

void FieldScene_RunActorPresentationSequence(void)
{
    extern u8 Data_02000240[];
    extern struct EventWork *Data_03001ebc;

    Func_02006f44(Data_02000240[0x205], Data_02000240[0x206]);
    Event_Begin();
    Call2(Func_02007164, 0x10000, 0x12c);
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 19);
    Actor_SetAnimation(8, 5);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    Call2(Func_020071a0, 0x18000, 16);
    Data_03001ebc->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    *(s32 *)((u8 *)Data_03001ebc + 0x1c8) = 16;
    Event_OpenScreen();
    Func_020071d4();
    Func_020071a0_a();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_Wait(20);
    Event_SetMessage(MSG_SUKURETA_HOW_LONG_WILL_ISLAND);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x107, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(8, 0x105, 80);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(8, 0x105, 100);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(80);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 80);
    Event_ShowMessageAndWait(8, 0, 120);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 40);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 120);
    Actor_SetSpeed(9, 0x6666, 0x3333);
    Actor_SetPosition(9, 0x1ddc0000, 0xd840000);
    Actor_WalkToAndWait(9, 0x1d94, 0xd8c);
    Actor_WalkToAndWait(9, 0x1d88, 0xda0);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x6009, 0, 20);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(0x6009, 0);
    Func_020070ec();
    Event_Wait(20);
    Actor_EnableActionCallback(9, 0x200cf7c);
    Event_Wait(80);
    Actor_SetAnimation(8, 1);
    Actor_Jump(8, 4, 40);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_Jump(ACTOR_JASMINE, 4, 60);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 40);
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(8, 0x200d01c);
    Event_Wait(20);
    Camera_SetSpeed(0xb333, 0x1666);
    Camera_MoveTo(0x1e380000, -1, 0xdc80000, 1);
    Engine_ActorEnableActionCallback(5, 0x200d0a8);
    do {
        Actor_SetAnimation(10, 6);
        Actor_SetAnimation(6, 8);
        Task_Wait(1);
    } while (*(s16 *)(Func_02007260(5) + 100) == 0);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x8000, 20);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Event_Wait(40);
    Func_020073fa();
    Call11(Engine_EventShowTwoMessagesAndWait, 5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Event_Wait(20);
    Func_02007202();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1e580000, -1, 0xdc80000, 1);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x19999, 0xcccc);
    Actor_WalkTo(8, 0x1e7c, 0xdb8);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1e6c, 0xdd8);
    Actor_SetAnimation(8, 1);
    Func_02007484();
    Event_Wait(80);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(0x1005, 0, 40);
    Actor_FaceDirection(8, 0x8000, 20);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 60);
    Func_020072a8();
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Func_0200749a((s32)Data_00000000, 10);
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    Event_Begin();
    Audio_PlayCue(141);
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Task_Wait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)0x0200b1fd;
        Func_02007328(transition_callback, transition_delay);
    }
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0x10004, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(40);
    Event_Wait(240);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(80);
    Task_Wait(90);
    Event_RequestExit(109);
    GameFlag_Set(282);
    Event_End();
}

enum {
    TRANSITION_EFFECT_TYPE = 222,
    TRANSITION_EFFECT_LEFT = 0x17b0,
    TRANSITION_EFFECT_TOP = 0x0c4c,
    TRANSITION_EFFECT_WIDTH = 40,
    TRANSITION_EFFECT_DEPTH = 30
};

/* The motion script each transition effect runs. */
extern const s32 Data_0200d14c[];

s32 UnsignedModulo(u32 value, s32 divisor);

/*
 * The timed callback the transition schedules. Each run places one effect at
 * a random point of a 40 by 30 pixel area with a random scale, and on every
 * third frame moves the camera to one of four nearby points chosen at random.
 */
void StoryScene_UpdateTransitionEffect(void)
{
    u32 x = (u32)Random_Next() * TRANSITION_EFFECT_WIDTH >> 16;
    u32 z = (u32)Random_Next() * TRANSITION_EFFECT_DEPTH >> 16;
    struct FieldActor *object;

    object = Object_Create(TRANSITION_EFFECT_TYPE, PIXELS(x) + PIXELS(TRANSITION_EFFECT_LEFT), 0,
                           PIXELS(z) + PIXELS(TRANSITION_EFFECT_TOP));
    if (object != NULL) {
        struct FieldSprite *sprite = object->sprite;
        s32 scale = (((u32)Random_Next() << 15) >> 16) + 0x13333;

        sprite->flags = 0;
        sprite->priority = 2;
        object->motion_flags = 0;
        object->scale_x = scale;
        object->scale_y = scale;
        Object_SetAnimation(object, 1);
        Object_SetScript(object, Data_0200d14c);
    }
    if (UnsignedModulo(gFrameCount, 3) == 0) {
        switch (((u32)Random_Next() << 2) >> 16) {
        case 0:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c69), 1);
            break;
        case 1:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c67), 1);
            break;
        case 2:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c69), 1);
            break;
        case 3:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c67), 1);
            break;
        }
    }
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{

    u32 i;
    s32 actor;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetAnimation(ACTOR, 2);
    Actor_SetPosition(ACTOR, 0x13080000, 0x3280000);
    actor = Actor_Get(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Task_Wait(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Camera_FollowActor(ACTOR, 1);
    Task_Wait(1);
    SCENE_PHASE = 0x100;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR, 0x6666, 0x3333);
    Actor_MoveToAndWait(ACTOR, 0x12d8, 0x2c8);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x268);
    Actor_SetSpeed(ACTOR, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x1d8);
    Actor_SetSpeed(ACTOR, 0x3333, 0x1999);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1c8);
    Actor_SetSpeed(ACTOR, 0x1999, 0xccc);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1b8);
    Actor_SetAnimation(ACTOR, 1);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    s32 positive_total = StoryReward_LookupBySelection(0);
    s32 negative_total;

    positive_total += StoryReward_LookupBySelection(2);
    negative_total = StoryReward_LookupBySelection(1);
    negative_total += StoryReward_LookupBySelection(3);
    return positive_total - negative_total;
}

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (GameFlag_IsSet(flag_base + offset) != 0) return Data_0200e718[offset];
    }
    return 0;
}

void FieldScene_RunScene371_0200357c(void)
{
    struct FieldActor *actor;
    s32 record;

    actor = (struct FieldActor *)Value1(Engine_ActorGet, 8);
    Event_Wait(60);
    Event_Begin();
    Call2(Func_02007a22, 0x9999, 1);
    actor->scale_x = 0x13333;
    actor->scale_y = 0x13333;
    Camera_FollowActor(8, 1);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    actor->unknown_64 = 0;
    Value2(Engine_ActorEnableActionCallback, 8, 0x200d218);
    Value2(Func_02007778, 0x200b4a1, 0xc80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_Wait(120);
    Call2(Func_02007aac, 0x16666, 0x12c);
    Event_Wait(0x10e);
    gEventWork->transition_frames = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{
    extern volatile s32 Data_03001e40;

    extern struct StorySelection Data_02000240;
    extern struct StoryProgressWork *Data_03001ebc;

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Actor_Get(Data_02000240.actor_id);
    scene = Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_0200793e(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_02007974(0x2f8, progress);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    s32 rec2;
    struct FieldActor *actor;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(s32 *)(base + 500);
    actor = (struct FieldActor *)Value1(Engine_ActorGet, (s32)p6);
    rec2 = GameFlag_IsSet(0x2f0);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Value1(Engine_ActorGet, a0);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Value2(Func_020078d6, 0x200b679, 0xc80);
        actor->motion_flags = rec2;
        Func_02007986(actor, actor->x.fixed, actor->y.fixed + 0x200000, actor->z.fixed);
        Actor_WaitForMove((s32)p6);
        actor->velocity_y = rec2;
        actor->motion_flags = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x2f0);
        Call2(Func_02007a38, 0x2f8, 180);
        Event_End();
        *(u16 *)(*(u8 **)Data_03001ebc + 0x17c) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    FieldScene_RunOpeningAuxiliarySequence(54);
}

void StoryScene_ActivateSharedState(void)
{
    Data_03001f30->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    extern u8 Data_02000240[];
    extern struct StoryCompletionWork *Data_03001ebc;

    u8 *selected_actor;

    if (Data_03001ebc->scene_value == 99) {
        Data_03001ebc->scene_value = 0;
    }
    GameFlag_Clear(0x2f0);
    GameFlag_Set(0x2f1);
    Func_02007ab2(0x2f8, 0);
    Func_02007c22(98, 5);
    Data_02000240[0x22b] = 3;
    Func_02007c34(98, 7);
    selected_actor = Actor_Get(*(s32 *)(Data_02000240 + 500));
    selected_actor[85] = 2;
}

/* Restore the blend registers using the active display bank's mask. */
void SceneEffect_RestoreBlendRegisters(void)
{
    extern u16 Data_0200e7a0;
    extern u32 Data_03001e40;

    Func_02007af0(0x04000050, 0x3f41);
    if ((Data_03001e40 & 2) != 0) {
        Func_02007b0a(0x04000052, Data_0200e7a0 | 0x0c);
    } else {
        Func_02007b34(0x04000052, Data_0200e7a0 | 0x10);
    }
}

void FieldScene_RunLateSequence(void)
{

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Actor_Get(Data_02000240_t[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Value2(Func_02007af8, *(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Random_Next() << 2) >> 16;
        switch (mode) {
        case 0:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Engine_CameraMoveTo((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Camera_MoveTo((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    extern s32 Data_03001e40;

    if ((Data_03001e40 & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007f98(obj);
    }
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        Object_SetPartPalettes(obj, Func_020080e0((s32)(Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007fd4(obj);
    }
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_0200811c((u32)Data_03001e40 >> 1, 6);

        Object_SetPartPalettes(arg0, slot);
    }
}

void StoryEffect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void StoryEffect_AnimateVerticalNegative(struct StoryVerticalEffectActor_02004004 *effect)
{
    struct StoryVerticalEffectActor_02004004 *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
