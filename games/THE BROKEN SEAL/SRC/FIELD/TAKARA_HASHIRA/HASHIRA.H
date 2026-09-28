#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"

#define NULL ((void *)0)
#define REC_ID 16
#define QUERY_FLAG 0x200

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"

/* The engine work slots; slot 36 holds the pillar scene's record. */
extern u8 gWorkSlot[];
#define PILLAR_WORK (*(u8 **)(gWorkSlot + 36 * 4))

/* The stage's tables in the overlay's read-only data. */
extern u8 TakaraHashira_ActionTable[];
extern u16 TakaraHashira_ShiftSteps1[];
extern u16 TakaraHashira_ShiftSteps2[];
extern u16 TakaraHashira_ShiftSteps3[];
extern u16 TakaraHashira_ShiftSteps4[];
extern u16 TakaraHashira_ShiftSteps5[];

/* The stage's callbacks, installed by address. */
void CopyAndOffsetCoordinatePreset(void);
void TakaraHashira_JitterBackgroundScroll(void);
s32 SceneActor_UpdateBit1ByPositionToSlotZero();
s32 TakaraHashira_UpdateActorPriority();

/* The veneers the shared staged-actor code names; the stage waits and sets
 * object modes through them too. */
void WaitFrames(s32 frames);
void Object_SetMode(struct FieldActor *object, s32 mode);

static inline void Stage_Wait(s32 frames)
{
    WaitFrames(frames);
}

static inline void Stage_SetMode(struct FieldActor *object, s32 mode)
{
    Object_SetMode(object, mode);
}
extern u8 TakaraHashira_ShakenScroll[];
extern u8 gEffectWork[];

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

typedef struct {
    s32 v[6];
} PlacementResult;

typedef struct {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
} Handle;

extern u8 gCell[][2];
extern s32 TakaraHashira_ShakeChance;


extern volatile s32 Data_03001e40;

s32 FieldScene_QueryActorFootprint();
void Vector_AddPolarOffset();
void Vector_AddPolarOffset();
s32 Object_CheckMovementCollision();
s32 Object_CheckMovementCollision();
void TakaraHashira_CopyCellBlock();
void TakaraHashira_CopyCellBlock();
void TakaraHashira_CopyCellBlock();
void TakaraHashira_CopyCellBlock();
void TakaraHashira_CopyCellBlock();
void TakaraHashira_CopyCellBlock();
void Runtime_SetIrqHandler();
void Runtime_SetIrqHandler();
s32 FieldScene_RunScene3b3SequenceD();
void TakaraHashira_UpdatePillarActors();
int FieldScene_RunScene3b3SequenceD(void);
void battle_owner_69();

/*
 * Staged actor height wait for overlay resource_3b3. The callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/*
 * Steps a record down by a tenth of a unit per frame until it reaches the
 * clamp at 0x1999 -- resource_3b3.  The eighty-byte owner includes its
 * alignment halfword and four pool words: the clamp 0x1999, -0x1999,
 * -0xcccc and the loop bound 0x1998, which is one less than the clamp.
 * None of them is an address, and each is reached only through a
 * pc-relative load.
 */

/* Apply the actor's low four counter bits as its animation mode. */

/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin);

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

static __inline__ s32 Value0(s32 (*f)())
{
    s32 FieldScene_RunScene3b3SequenceD();

    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 FieldScene_RunScene3b3SequenceD();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 FieldScene_RunScene3b3SequenceD();

    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    s32 FieldScene_RunScene3b3SequenceD();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

