#ifndef ALCHEMY_ENTRANCE_H
#define ALCHEMY_ENTRANCE_H

/* Declarations shared by the Mercury Lighthouse entrance scene sources. */
#include "TYPES.H"
#include "MAKYURI.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "STAGED_ACTOR.H"

/* The effect record an overlay object carries at +0x50. */
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

/* A rising effect and the object it stays anchored to. */
struct Src_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct Src_39c *f104;       /* +104 */
};

/* The IWRAM work pointers from gWork: the event work, and 0x20 bytes on, the
 * scene's. */
struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};

extern struct WorkPointers gWork;

/* Actor 8's action tables, scripts and scene tables in the overlay's data. */
extern const u8 MakyuriIriguchi_Actor8Path1[];
extern const u8 MakyuriIriguchi_Actor8Path2[];
extern const u8 MakyuriIriguchi_Actor8Path3[];
extern const s32 MakyuriIriguchi_RiseScript[];
extern const s32 MakyuriIriguchi_SparkBurstScript[];
extern const s32 Makyuri_ScaleCounterScript[];
extern u8 MakyuriIriguchi_SceneTableA[];
extern u8 MakyuriIriguchi_SceneTableB[];
extern u8 MakyuriIriguchi_SceneTableC[];
extern u8 MakyuriIriguchi_SceneTableD[];
void Makyuri_CyclePalette(void);
void Effect_Spawn();

void AnimationObjects_SelectAnimation();
u8 **Runtime_AllocateBlock(s32, s32);
void BattleFx_PlayQueuedSound();
u16 ArcTan2(s32 deltaZ, s32 deltaX);
void Makyuri_TickSpawnTimer(void);
void Makyuri_RunActorMove(void);
void SceneState_ClearCurrentRecordAndReleaseTarget(void);
void MakyuriIriguchi_RaisePillar(void);
void SceneActor_RunActorZeroHandledMotion(int page);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Functions of the overlay's other sources. */
void SetEffectRecordMode(struct EffectWork *work, s32 mode);
void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 MakyuriIriguchi_TrailSparks(s32 a0);
s32 SceneAudio_PlayCue118AndReturnZero(void);
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object);
void FieldScene_Forward31d4(void);
void FieldScene_CallHelper2e58(void);
void FieldScene_RunSingleStep(void);
void FieldScene_CallHelper2d50(void);
void FieldScene_RunStepWithValue1632(void);
void MakyuriIriguchi_DropLeaderToColumn(s32 a0);
void FieldScene_RunIndexedStep17(void);
void FieldScene_RunIndexedStep18(void);
void FieldScene_RunIndexedStep19(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunIndexedStep63(void);
void FieldScene_RunActor8StepWithTableA820(void);
void MakyuriIriguchi_SendActor8ByLeaderColumn(void);
void MakyuriIriguchi_ArriveWithSparks(void);
void MakyuriIriguchi_RunDoorScene(void);
void OverlayObject_SpawnKind24AtObject(u8 *src);
s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work);
s32 OverlayObject_RampWords24And28Over16Frames(u8 *obj);
s32 OverlayObject_AdvanceScaleCounter(u8 *o);
void OverlayObject_ReleasePublishedAttachment(void);
void MakyuriIriguchi_CrossDoorway(void);

#endif
