#ifndef ALCHEMY_UNIT_FIELD_KORIMA_KAWA_STAGED_PALETTE_H
#define ALCHEMY_UNIT_FIELD_KORIMA_KAWA_STAGED_PALETTE_H

#include "types.h"

struct StagedActorEffectRequest;

u8 *MapStagedScene_SelectPrimaryData(void);
s32 MapStagedScene_GetEmptyData(void);
u8 *MapStagedScene_SelectSecondaryData(void);
u8 *MapStagedScene_SelectTertiaryData(void);
void FieldScene_RunActorTenPlacementScene(void);
s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request);
void SceneActor_ApplyOffsetObjectPosition(void);
u8 *KorimaKawa_GetEvents(void);
s32 KorimaKawa_PrepareWindowTransition(void);
void SceneEffect_AdjustPaletteColors(s32 a);
u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj);

#endif
