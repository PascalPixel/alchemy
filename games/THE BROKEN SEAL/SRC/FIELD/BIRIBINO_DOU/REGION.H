#ifndef BIRIBINO_DOU_REGION_H
#define BIRIBINO_DOU_REGION_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern s32 StagedActor_DirectionSteps[];

void FieldScene_RunScene398SequenceB(void);
void RunGuardedSceneSetup(void);
void BiribinoDou_PrepareWindowTransition(void);
void FieldScene_RunScene398SequenceC(void);
void Resource398_ImportBankNoOp();

#endif
