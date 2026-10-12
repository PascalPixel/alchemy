#ifndef ALCHEMY_UNIT_SCENE_SORU_STAR_H
#define ALCHEMY_UNIT_SCENE_SORU_STAR_H

#include "types.h"
#include "field_scene.h"

s32 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
s32 *SceneData_GetMessageTable(void);
s32 *SceneData_GetActorTable(void);
s32 *SceneData_GetEffectTable(void);
s32 SoruStar_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 SoruStar_AngleScript[];
extern s32 SoruStar_PresentItemScript[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern u16 SoruStar_StarCells[];
extern u8 Soru_RingDrift[][3];
extern u8 Soru_RingSwing[][3];
extern s8 Soru_RingDirection[][3];
extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];

#endif
