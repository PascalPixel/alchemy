#ifndef BIRIBINO_DOU_REGION_H
#define BIRIBINO_DOU_REGION_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

/*
 * The Vault cave's scene code: its map regions, the gate and switch scenes,
 * the entrance setup and the pushed statues.
 */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/* The packed direction steps, in the overlay's read-only data: indexed by
 * heading >> 12, X step in the high halfword and Z step in the low. */
extern s32 StagedActor_DirectionSteps[];

void FieldScene_RunScene398SequenceB(void);
void RunGuardedSceneSetup(void);
void SceneState_SetRuntimeWord448To516(void);
void FieldScene_RunScene398SequenceC(void);
void Resource398_ImportBankNoOp();

#endif
