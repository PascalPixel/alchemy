/* Declarations shared by Mogall Forest's waypoint sources (resource_39f). */
#ifndef MORI_H
#define MORI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "FIELD_EFFECT.H"
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

enum WaypointMessage {
    MSG_BROKEN_SIGN_READS_NORTH_FUCHIN = 0x17e6
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

/* Main-image code the import veneers reach, under the names the staged-actor
 * module gives those veneers in this overlay. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Object_SetModeById(s32 actor, s32 mode);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
s32 Object_CheckMovementCollision(s32 *p, s16 *q);
void BattleFx_SetPhaseRequest(s32 a, s32 b);
void BattleFx_SetWeightedResult();
u8 *AllocateEffectTransfer(s32, s32);
void MogoruMori_SpawnPuffRing(s32 id);

/* The scene table in the overlay's data. */
extern u8 MogoruMori_SceneTable[];

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

/* 0x02003fb6 serves the two-argument presentation select and, twice later,
 * one-argument imports; the `_b` alias carries the two-argument one. */

/* An EWRAM global, not overlay data: the module is linked above this address,
 * so it cannot be in-image. */

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */

/* Four register arguments plus four stack words. */

/* 0x02004246 serves two imports: the one-argument post at site 5 and the
 * six-argument message at site 12. */

/* 0x02004d98 serves two imports: the two-argument setter at site 7 and the
 * three-argument one at site 14. */

/* 0x02004f28 serves two imports: the two-argument setter at site 7 and the
 * three-argument step at site 14. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The sources call one another across files. */
void Func_020009dc(u8 *object, s32 mode);
u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind);
void SceneState_SetValue18Mode2(void);
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg);
s32 OverlayObject_ApplyField100(s32 a);
s32 OverlayObject_ApplyZero(s32 a);
void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3);
void FieldScene_RunSixCallSetupSequence(s32 no, s32 val);

s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTableB5bc(void);

void SceneActor_PassOffsetPointOfActorZero(void);
void SceneActor_BobActorZeroWhenAheadClear(void);
void FieldScene_RunScriptedSteps0And17E6(void);
void FieldScene_RunActor10WaypointSequence(void);
void FieldScene_RunActorElevenPresentationBeat(void);
void SceneActor_RunActorTwelveThreeWaypointMotion(void);
void FieldScene_RunStepFD4WithActor181(s32 a);
void FieldScene_RunProbedActorEightOrTenScene(void);
void SceneState_ApplyCrossRectsAroundActor11(void);
void FieldScene_RunActorThirteenPresentationBeat(void);
void FieldScene_RunScene39f_02001818(void);
void SceneActor_RunActorFourteenFourWaypointMotion(void);
void SceneActor_BobActorZeroWhenTargetClear(void);
void SceneActor_MarkActorThirteenTileAndPark(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunScene39fSequenceA(void);
void FieldScene_RunSlot16WaypointSequence(void);
void FieldScene_RunActor17CameraSequence(void);
void FieldScene_RunScene39f_02002004(void);
void FieldScene_RunActorEighteenEffectSequence(void);
void FieldScene_RunScene39f_020021b0(void);

s32 UpdateOrbitingSceneObject(s32 *p);
void InitializeOrbitingEffect(s32 id);

#endif
