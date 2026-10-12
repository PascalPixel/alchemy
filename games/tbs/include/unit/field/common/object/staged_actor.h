#ifndef ALCHEMY_UNIT_FIELD_COMMON_OBJECT_STAGED_ACTOR_H
#define ALCHEMY_UNIT_FIELD_COMMON_OBJECT_STAGED_ACTOR_H

#include "types.h"
#include "staged_actor.h"

struct StagedActor;
struct StagedActorProbe;

struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin);
void StagedActor_AdvancePair(void);
s32 StagedActor_StopBlockedMotion(struct StagedActor *actor);
u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint);
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);
void SceneActor_MoveAndRedraw(struct StagedActorProbe probe);
s32 FieldScene_RedrawActorFootprint(s32 id);

#endif
