#ifndef MAKYURI_CHOJO_AERIE_H
#define MAKYURI_CHOJO_AERIE_H
extern struct EventWork *gEventWork;

#if defined(TBS_EDITION_JA)
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x1725
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x157e
#else
#define MSG_LIGHTHOUSE_ALREADY_LIT 0x159c
#endif

extern u8 *gWork;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* Unresolved declarations retain their existing call signatures. */
s32 Battle_GetWorkObject1e0Far();
void Event_SetValue1d8Far();
void Object_LinkPairFar();
s32 UiText_OpenMessageAtObjectFar();
void Object_LinkPairFar();
void MakyuriChojo_LowerCollectedActors();
void Object_LinkPairFar();
void ObjectMotion_SetPositionAndCommitFar();
s32 UiText_OpenMessageAtObjectFar();

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

extern u8 *gWork;
extern u8 *gCam;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define EVENT_TRANSITION(workspace) FIELD((workspace), s32 *, 0x1C0)
#define EVENT_MESSAGE(workspace) FIELD((workspace), u16 *, 0x1D8)

extern void RunOverlayObjectCommand1();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
void MakyuriChojo_SinkActorPair();
void Event_SetValue1d8Far();
s32 Battle_GetWorkObject1e0Far();
void MakyuriChojo_RiseActorPair();
s32 UiText_OpenMessageAtObjectFar();
void MakyuriChojo_SinkActorPair();
void MakyuriChojo_RiseActorPair();
void MakyuriChojo_RaiseCollectedActors();
void Object_LinkPairFar();
s32 UiText_OpenMessageAtObjectFar();
void MakyuriChojo_LowerCollectedActors();
s32 Battle_GetWorkObject1e0Far();
s32 UiText_OpenMessageAtObjectFar();
s32 UiText_OpenMessageAtObjectFar();
s32 UiText_OpenMessageAtObjectFar();
s32 UiText_OpenMessageAtObjectFar();
void Audio_PlayCueFromEventWorkFar();

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void RunOverlayObjectCommand1(void *, s32);

extern u8 MakyuriChojo_ScriptTable[];

struct Record {
    u8 pad00[85];
    u8 mode55;              /* +85, 0x55 */
};

extern u8 MakyuriChojo_MessageTable[];
extern u8 MakyuriChojo_ActorTable[];
extern u8 MakyuriChojo_EventTable[];

void BattleEffect_CleanupSceneObjectsFar(void);

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */

#define SCENE_PHASE (*(s32 *)(*(u8 **)&gEventWork + 0x1c0))
#define ACTOR_FIELD_0XA(record) (*(s16 *)((record) + 10))
#define ACTOR_FIELD_0X12(record) (*(s16 *)((record) + 18))

extern u8 MakyuriChojo_NearestActor[];

void Owner_RefreshRatiosOnFlagFar();
s32 Battle_GetWorkObject1e0Far();
void MakyuriChojo_RaiseCollectedActors();
s32 Battle_GetWorkObject1e0Far();
void MakyuriChojo_LowerCollectedActors();

/* Newly named call sites: the mechanical pass left these spelled raw because
 * their calling form (a cast, or an odd argument count) did not match its
 * patterns, or because the engine function itself had no name yet. */

/* Status/phase word at 0x1c0 of the shared scene work record. */

/* Two s16 fields (offsets 0xa and 0x12) read together from an actor slot
 * record and forwarded as a pair of arguments. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Configures four actor slots (0-3), advances the shared scene phase, then
 * drives handles 1-3 through a sequence of timed calls, some of which use
 * the two s16 fields read back from an actor slot record. */

typedef s32(*IwramIntegerSquareRoot)(s32);

typedef struct {
    u8 pad_to_kind[0x28];
    s16 *kind;
} ActorData;

typedef struct {
    u8 pad_to_data[0x50];
    ActorData *data;
} Actor;

#endif
