#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define CreateOverlayObject Func_02001132
#define SetOverlayObjectMode Func_0200117c
#define SetOverlayObjectSlot Func_02001244
void Effect_Move(void *object);
#define Actor_CheckProximity SceneActor_UpdateProximity
#define Actor_UpdateProximity SceneActor_UpdateProximity

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

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

struct SceneActor_02000350 {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_020004b4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct Presentation_02000c1c {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor_02000c1c {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation_02000c1c *presentation;
};

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern u8 SceneMessage_ActorFourteenBase;
extern const unsigned char Data_02009450[];
extern const unsigned char Data_020095a0[];
extern const unsigned char Data_020095d8[];
extern const u8 Data_020097dc[];
extern struct Actor_020007d4 *Func_020018e0(s32 actor);  /* Scene_GetRecord */
extern struct Actor_020007d4 *Func_020018f2(s32 actor);  /* Scene_GetRecord */
extern const u8 Data_02009756[];
extern struct Actor_02000754 *Func_02001860(s32 actor);  /* Scene_GetRecord */
extern struct Actor_02000754 *Func_02001872(s32 actor);  /* Scene_GetRecord */
extern u8 Data_00000002[];
extern u8 Data_00001cc0[];
extern const u8 Data_02009740[];
extern const u8 Data_0200976c[];
extern const u8 Data_020097ae[];
extern s16 SceneState_Table[];

s32 Func_020019c0();
s32 Func_02001a1e_a();
s32 Func_02001a44_a();
s32 Func_02001a56();
typedef s32(*IwramIntegerSquareRoot)(s32);
s32 Func_0200068c(s32 *, s32 *);
u32 Func_020013fc(s32, s32);
s32 Func_02001f80(s32 cue);
s32 Func_0200190a();
void Func_020013bc();
void Func_02001f26(s32 actor, s32 mode, s32 value);
void Func_02001e62();
s32 SceneFlag_Check(s32 flag);
void SceneFlag_Set(s32 flag);
void SceneMap_UpdateRect(s32, s32, s32, s32, s32, s32);
void SceneMap_CopyRect(s32, s32, s32, s32, s32, s32);
void SceneState_SetMode(s32 mode);
u8 *SceneActor_Find(s32 actor);
void SceneActor_Place(s32 actor, s32 x, s32 z);

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
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */

/* Complete scene-variant six actor-zero setup wrapper through its pool. */

/* Complete actor-mode scene wrapper through its return and alignment. */

/* Complete paired scene-cell presentation update. */

/* Complete alternate paired scene-cell presentation update. */

/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Func_080091c0/Func_08009180,
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
    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    func(a, b, c);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

/*
 * Complete actor-21 mode-four reset wrapper.
 *
 * Call symbol resolved directly against the raw region in
 * recon/tbs/raw/overlays/resource_385_overlay. s(`push {lr} / movs r0,#21 / movs r1,#0 /
 * movs r2,#4 / bl sub_02001f26 / pop {r0} / bx r0`): the per-site overlay
 * veneer is sub_02001f26, not a main-image address guessed from elsewhere.
 */

static __inline__ void Call1_02000640(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call1_02000d74(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void PlaceActor(void (*place)(s32, s32, s32),
                                 s32 actor, s32 x, s32 z)
{

    place(actor, x, z);
}

static __inline__ void UpdateRect(void (*update)(s32, s32, s32, s32, s32, s32),
                                 s32 x, s32 z, s32 width, s32 height,
                                 s32 sourceX, s32 sourceZ)
{

    update(x, z, width, height, sourceX, sourceZ);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = Object_Create(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 0xE);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObjectB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

s32 Func_02000314(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_UpdateProximity(struct SceneActor_02000350 *actor, struct SceneActor_02000350 *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Func_0200068c(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Func_020013fc(target->z - actor->z,
                                      *targetPos - *actorPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Object_SetAnimation(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Object_SetAnimation(actor, 2);
    }
    return result;
}

s32 UpdateActorProximity(u8 *actor)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(actor + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Actor_Get(15);
    } else {
        partner = Actor_Get(14);
    }
    if (Actor_CheckProximity(actor, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_Get(0);

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
    return Data_02009450;
}

/* Complete zero-return leaf; no calls and no argument read. */
int SceneData_ReturnZero(void)
{
    return 0;
}

const void *SceneData_GetTable95A0(void)
{
    return Data_020095a0;
}

const void *SceneData_GetTable95D8(void)
{
    return Data_020095d8;
}

void ActorPresentation_RunActorModeOneThenZero(s32 actor)
{
    Event_Begin();
    Actor_SetAnimation(actor, 1);
    Event_ShowMessage(actor, 0);
    Event_End();
}

void SceneDialogue_RunActor8FlagScene(void)
{
    Event_Begin();
    Actor_FaceActor(8, 0, 2);
    GameFlag_Set(0x305);
    Event_SetMessage(0x1cab);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActor11Line(void)
{
    /*
 * Complete actor-11 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * recon/tbs/raw/overlays/resource_385_overlay. s(lines 753-765, still unadopted at the
 * time of writing): `bl sub_0200167a`, `bl sub_02001674`, then
 * `bl .L_020009d6` -- the third call targets a local branch label at
 * 0x020009d6, not the guessed 0x020004b4.
 */

    Event_SetMessage(0x1cae);
    Actor_FaceEachOther(11, 0, 2);
    ActorPresentation_RunActorModeOneThenZero(11);
}

void SceneDialogue_RunActor12TwoFlagScene(void)
{
    /*
 * Complete actor-12 two-flag dialogue scene through its three-word pool.
 *
 * Call symbols are per-site, not per-import: the raw region in
 * recon/tbs/raw/overlays/resource_385_overlay. s(lines 766-783, still unadopted) spells
 * every call as `bl sub_02000xxx`, and the two calls that both ultimately
 * reach GameFlag_Set go through TWO DIFFERENT overlay-local veneers,
 * sub_02001608 and sub_0200160e -- not the same symbol twice.
 */

    Event_Begin();
    Actor_FaceActor(12, 0, 2);
    GameFlag_Set(0x306);
    GameFlag_Set(0x868);
    Event_SetMessage(0x1caf);
    Event_ShowMessage(12, 0);
    Event_End();
}

void SceneDialogue_ShowLine1CB0ForActor13(void)
{
    /*
 * Complete actor-13 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * recon/tbs/raw/overlays/resource_385_overlay. s(lines 757-769, still unadopted at the
 * time of writing): `bl sub_020016da`, `bl sub_020016d4`, `bl sub_02000a36`.
 */

    Event_SetMessage(0x1cb0);
    Actor_FaceEachOther(13, 0, 2);
    ActorPresentation_RunActorModeOneThenZero(13);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    struct SceneActor *actor = Actor_Get(14);
    s16 facing = (s16)actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Event_Begin();
    text = (s32)&SceneMessage_ActorFourteenBase;
    Event_SetMessage(text);
    Actor_SetAnimation(14, 0);
    Actor_FaceEachOther(14, 0, 2);
    if (Scene_QueryFlag(Engine_GameFlagIsSet, 0x300) == 0) {
        Scene_Call3(Engine_ActorShowEmote, 14, 256, 60);
        Event_ShowMessageAndWait(14, 0, 10);
        Event_ShowMessageAndWait(14, 0, 10);
        Scene_SetFlag(Engine_GameFlagSet, 0x300);
    }
    Event_SetMessage(text + 2);
    Event_ShowMessageAndWait(14, 0, 10);
    actor->facing = facing;
    Task_Wait(1);
    Event_End();
    {
        s32 shown = 1;
        actor->state_flags = shown;
    }
    GameFlag_Set(0x307);
}

void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void)
{
    struct Actor_02000640 *actor;
    s16 facing0;

    actor = Actor_Get(15);
    facing0 = (s16)actor->facing;
    actor->state_flags |= 2;
    Event_Begin();
    Event_SetMessage(0x1cb4);
    Actor_SetAnimation(15, 0);
    Actor_FaceEachOther(15, 0, 2);
    Event_ShowMessageAndWait(15, 0, 10);
    actor->facing = (u16)facing0;
    Task_Wait(1);
    Event_End();
    actor->state_flags = 0;
}

void SceneDialogue_RunActor16CountedDialogue(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Event_Begin();
    Event_SetMessage(0x1cb5);
    Actor_FaceEachOther(16, 0, 2);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Data_03001ebc->branch_counter += 1;
    }
    Event_ShowMessage(16, 0);
    GameFlag_Set(0x308);
    Event_End();
}

void SceneDialogue_RunActorEightTimedDialogue(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_FaceActor(8, 0, 20);
    GameFlag_Set(0x305);
    Event_SetMessage(0x1cab);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_End();
}

void SceneDialogue_RunActor11AcceptanceDialogue(void)
{
    Event_SetMessage(0x1cbd);
    Func_02001860(11)->accepted = 1;
    ActorPresentation_RunActorModeOneThenZero(11);
    Func_02001872(11)->accepted = 0;
}

void SceneDialogue_RunActor12TimedTwoFlagScene(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    Actor_FaceActor(12, 0, 20);
    GameFlag_Set(0x306);
    GameFlag_Set(0x868);
    Event_SetMessage(0x1caf);
    Event_ShowMessageAndWait(12, 0, 20);
    Event_End();
}

void ActorPresentation_RunActor13AcceptanceDialogue(void)
{
    Event_SetMessage(0x1cbf);
    Func_020018e0(13)->accepted = 1;
    ActorPresentation_RunActorModeOneThenZero(13);
    Func_020018f2(13)->accepted = 0;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    u8 *record;

    {
        u16 *flags = (u16 *)(Func_0200190a(14) + 100);
        u16 bits = (u16)(s32)Data_00000002;

        *flags |= bits;
    }
    Event_Begin();
    if (GameFlag_IsSet(0x307) != 0) {
        Event_SetMessage((s32)Data_00001cc0);
        ActorPresentation_RunActorModeOneThenZero(14);
    } else {
        SceneDialogue_RunActorFourteenFlagDialogue();
        GameFlag_Set(0x307);
    }
    Event_End();
    {
        u8 *record = Actor_Get(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}

void SceneDialogue_RunActorFifteenDialogue(void)
{
    {
        struct SceneActor *actor = Actor_Get(15);
        actor->state_flags |= 2;
    }
    Event_Begin();
    Event_SetMessage(0x1cc1);
    ActorPresentation_RunActorModeOneThenZero(15);
    Event_End();
    {
        s32 clear = 0;
        struct SceneActor *actor = Actor_Get(15);
        actor->state_flags = clear;
    }
}

void FieldScene_RunScene385SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = GameFlag_IsSet(0x308);
    if (rec7 == 0) {
        Event_Begin();
        *(u8 *)(Func_020019c0(16) + 91) = 1;
        Actor_SetAnimation(16, 1);
        Call2((void (*)())Engine_ActorRunRepeatedMotion, 16, 1);
        Event_Wait(20);
        Event_SetMessage(0x1cb5);
        Call3((void (*)())Engine_ActorFaceEachOther, 16, 0, 2);
        Event_OpenMessage(16, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
            bump_step(1);
        }
        Event_ShowMessage(16, 0);
        *(u8 *)(Func_02001a1e_a(16) + 91) = rec7;
        Actor_EnableActionCallback(16, 2);
        Call0(Engine_EventEnd);
        GameFlag_Set(0x308);
    } else {
        Event_SetMessage(0x1cc2);
        *(u8 *)(Func_02001a44_a(16) + 91) = 1;
        ActorPresentation_RunActorModeOneThenZero(16);
        v5 = 0;
        *(u8 *)(Func_02001a56(16) + 91) = v5;
    }
}

void Func_02000970(int actor, int x, int z, int field40)
{
    u8 *record = Actor_Get(actor); int frames;
    Actor_SetSpeed(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Actor_SetSpriteFlags(record, 0);
    Actor_MoveToAndWait(actor, x, z); Actor_SetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Task_Wait(1); if (*(s16 *)(record + 42) == 0) break; }
    Actor_SetSpriteFlags(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunScene385_020009f8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(100);
    Event_Wait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(21, 0x102);
        Actor_Jump(21, 4, 0);
        Event_Wait(12);
        Actor_Jump(21, 4, 0);
        Event_Wait(20);
        Call4(Func_020013bc, 21, 0x188, 104, 0x70000);
        Event_Wait(20);
        Actor_WalkToAndWait(21, 0x198, 104);
        Actor_WalkToAndWait(21, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Event_End();
}

void PlaceActorAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
    s32 zero = 0;

    SetScale(zero, 0x8000, 0x4000);
    Actor_WalkTo(zero, x, y);
    gEventWork->transition_frames = 16;
    Event_RequestExit(delay);
}

void FieldScene_SetupDescriptor9740(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(Data_02009740, 56, 19);
    PlaceActorAndSetSceneDelay(408, 320, 5);
}

void ActorPresentation_SetupActorEighteenAt312_304(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(Data_02009756, 50, 18);
    PlaceActorAndSetSceneDelay(312, 304, 6);
}

void FieldScene_SetupWithDescriptor976C(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(Data_0200976c, 44, 17);
    PlaceActorAndSetSceneDelay(216, 288, 7);
}

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
    struct SceneActor_020004b4 *actor = Actor_Get(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x02009782, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    PlaceActorAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
    struct SceneActor_020004b4 *actor = Actor_Get(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x02009798, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    PlaceActorAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupWithDescriptor97AE(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(Data_020097ae, 38, 6);
    PlaceActorAndSetSceneDelay(120, 144, 10);
}

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
    struct SceneActor_02000c1c *actor = Actor_Get(0);
    struct Presentation_02000c1c *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x020097c4, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    PlaceActorAndSetSceneDelay(72, 160, 12);
}

void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
    GameFlag_Set(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Map_CopyCellAttributes(55, 26, 4, 2, a, b);
    }
}

void ActorPresentation_SetFlag200AndSceneCell23(void)
{
    GameFlag_Clear(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Map_CopyCellAttributes(23, 23, 4, 2, first_value, second_value);
    }
}

void FieldScene_SetActor21Values0And4(void)
{
    Func_02001f26(21, 0, 4);
}

void FieldScene_RunScene385_02000d84(void)
{
    u32 i;
    s32 record;

    Func_02001e62(231);
    Event_Begin();
    Event_Wait(10);
    Actor_RunRepeatedMotion(18, 2);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 216, 0x198);
    Event_Wait(10);
    Actor_FaceDirection(18, 0x4000, 20);
    Actor_Jump(18, 6, 0);
    Event_Wait(30);
    Actor_Jump(18, 6, 0);
    Event_Wait(30);
    Actor_Jump(18, 6, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(18, 216, 0x188);
    Event_Wait(10);
    Actor_FaceDirection(18, 0x4000, 20);
    GameFlag_Set(0x858);
    Event_End();
}

void ActorPresentation_SetPairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Map_CopyCellAttributes(41, 43, 1, 1, v1, v2);
    Map_CopyCellsTo(40, 42, 12, 22, 3, 3);
}

void ActorPresentation_SetAlternatePairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Map_CopyCellAttributes(37, 43, 1, 1, v1, v2);
    Map_CopyCellsTo(36, 42, 12, 22, 3, 3);
}

void FieldScene_RunActorEighteenDialogue(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Event_Begin();
    Event_SetMessage(0x1342);
    Actor_SetAnimation(18, 0);
    Actor_FaceEachOther(18, 0, 0);
    Event_Wait(2);
    Event_ShowMessage(18, 0);
    Actor_SetAnimation(18, 1);

    if (Func_02001f80(231) != -1 && GameFlag_IsSet(0x858) == 0) {
        Data_03001ebc->actor18_marker = 1;
    }

    Event_End();
}

void SceneState_SetFlag947AndValue29dc(void)
{
    Event_Begin();
    Message_ShowCentered(0x947, 1);
    Message_ShowCentered(0x29dc, 1);
    Event_End();
}

const u8 *SceneData_GetTable97dc(void)
{
    return Data_020097dc;
}

s32 SceneSetup_InitializeActorsAndFlags(void)
{

    u8 *actor;
    s16 *scene;
    s32 mode;

    if (SceneFlag_Check(0x200))
        UpdateRect(SceneMap_UpdateRect, 55, 26, 4, 2, 23, 26);
    OverlayObject_CreateConfiguredObjectB(0x800000, 0, 0x1a40000, 223);
    SceneMap_CopyRect(45, 41, 8, 45, 3, 3);
    SceneState_SetMode(1);
    actor = SceneActor_Find(14);
    *(u32 *)(actor + 108) = 0x02008401;
    {
        u8 *actor = SceneActor_Find(14);
        s32 mode = 1;
        *(u16 *)(actor + 100) = mode;
    }
    mode = 0;
    actor = SceneActor_Find(15);
    *(u32 *)(actor + 108) = 0x02008401;
    *(u16 *)(SceneActor_Find(15) + 100) = mode;
    if (SceneFlag_Check(0x858))
        PlaceActor(SceneActor_Place, 18, 0xd80000, 0x1880000);
    if (SceneState_Table[225] <= 2 && !SceneFlag_Check(52) && !SceneFlag_Check(0x109))
        SceneFlag_Set(0x867);
    if (SceneFlag_Check(0x867) && !SceneFlag_Check(52))
        PlaceActor(SceneActor_Place, 21, 0x1980000, 0x780000);
    scene = SceneState_Table;
    if (scene[225] == 11)
        SceneFlag_Set(FLAG_ARRIVAL_EVENT_PENDING);
    if (scene[225] == 13)
        SceneFlag_Set(0x120);
    return 0;
}
