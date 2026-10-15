#ifndef ALCHEMY_UNIT_SCENE_SOL_STAR_H
#define ALCHEMY_UNIT_SCENE_SOL_STAR_H

#include "types.h"
#include "field_scene.h"

s32 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
s32 *SceneData_GetMessageTable(void);
s32 *SceneData_GetActorTable(void);
s32 *SceneData_GetEffectTable(void);
s32 SolStar_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 SolStar_AngleScript[];
extern s32 SolStar_PresentItemScript[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern u16 SolStar_StarCells[];
extern u8 Sol_RingDrift[][3];
extern u8 Sol_RingSwing[][3];
extern s8 Sol_RingDirection[][3];
extern s32 Sol_RingOffsetX[];
extern s32 Sol_RingOffsetZ[];

#endif
