#ifndef ALCHEMY_UNIT_FIELD_GOMA_SUIRO_SUIRO_H
#define ALCHEMY_UNIT_FIELD_GOMA_SUIRO_SUIRO_H

#include "types.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SceneEffect_SetRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunActor8AtCell24Sequence(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_PlaceActor8OnEntry(void);
void SceneActor_WaitObjectBelowHeight(u8 *object, s32 height);
void SceneEffect_RunActorBurst(s32 no);

#endif
