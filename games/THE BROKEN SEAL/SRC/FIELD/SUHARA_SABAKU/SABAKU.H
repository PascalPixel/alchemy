#ifndef SUHARA_SABAKU_SABAKU_H
#define SUHARA_SABAKU_SABAKU_H

/* The Suhara desert (resource_3c0): the declarations, views and call
 * helpers its scene sources share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

extern u8 gEffectWork[];

struct Selection_02000400 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02000400 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02000400 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern u8 gSuharaSabakuEventsA6[];
extern u8 gSuharaSabakuExits[];
extern u8 gSuharaSabakuActor12Action[];

extern u16 gSuharaSabakuShownLevel;
s32 GameFlag_GetByte();
void GameFlag_SetByte();
void BattleFx_SetPhaseRequest();
void BattleFx_EmitRandomParticle();
extern u8 gSuharaSabakuEvents[];

#define ACTOR_ID 13

/* Actor id operated on throughout this scene. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

#define EncounterPalette (*(volatile u16 *)0x0500019e)

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

void SetEffectRecordMode(struct EffectWork *work, s32 mode);

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void SceneState_SendRequest15With45(void);

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj);

s32 OverlayObject_ToggleField84Bit0(u8 *obj);

s32 SceneData_SelectDataByRuntimeSelector(void);

s32 SceneData_ReturnZero(void);

u8 *SuharaSabaku_GetExits(void);

s32 SceneData_SelectOverlayDataByRuntimeSelector(void);

void SuharaSabaku_SyncSelectedActorProgress(void);

void FieldScene_RunMiddleAuxiliarySequence(s32 a0);

void FieldScene_RunActor8Step(void);

void FieldScene_RunActor9Step(void);

void FieldScene_RunActor10Step(void);

void FieldScene_RunActor11Step(void);

void FieldScene_RunActor12Step(void);

void FieldScene_RunLateActor8Step(void);

void FieldScene_RunLateActor9Step(void);

void FieldScene_RunLateActor10Step(void);

void FieldScene_RunLateActor11Step(void);

void FieldScene_RunLateActor12Step(void);

void FieldScene_RunActorThirteenRestoration(void);

void PlaceActorTwelveWhenFlagClear(void);

void SceneState_SetStateByte52(void);

void EncounterPalette_Pulse(void);

s32 FieldScene_RunOpeningAuxiliarySequence(s32 a0);

s32 FieldScene_RunScene3c0SequenceA(s32 a0);

s32 SuharaSabaku_SelectEvents(void);

#endif
