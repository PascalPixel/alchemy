#ifndef BIRIBINO_DOU_REGION_H
#define BIRIBINO_DOU_REGION_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "map_render_work.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern s16 StagedActor_DirectionSteps[];

void FieldScene_RunScene398SequenceB(void);
void RunGuardedSceneSetup(void);
void BiribinoDou_PrepareWindowTransition(void);
void FieldScene_RunScene398SequenceC(void);
void Resource398_ImportBankNoOp();

#endif
