#ifndef ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
s32 SolEruption_RunHostageSceneOnEntranceTen(void);
extern s32 Eruption_ArcOriginsHead[];
extern s32 Eruption_ArcOrigins[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern u16 SolEruption_StarCells[];
extern s32 Eruption_EmberScript[];
extern u8 Sol_RingDrift[][3];
extern u8 Sol_RingSwing[][3];
extern s8 Sol_RingDirection[][3];
extern s32 Sol_RingOffsetX[];
extern s32 Sol_RingOffsetZ[];

#endif
