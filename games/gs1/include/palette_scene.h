#ifndef ALCHEMY_PALETTE_SCENE_H
#define ALCHEMY_PALETTE_SCENE_H

#include "types.h"

u8 *PaletteScene_GetScriptData(void);
u8 *PaletteScene_GetMessageData(void);
u8 *PaletteScene_GetActorData(void);
u8 *PaletteScene_GetEffectData(void);
void PaletteScene_Initialize(void);
u8 *PaletteScene_GetState(void);
void PaletteScene_RunActorNineBranch(void);
void PaletteScene_RunActorEightBranch(void);
void PaletteScene_RunFlaggedBranch(void);
s32 PaletteScene_AdvanceEffectFrame();
void PaletteScene_SpawnEffect(void);
void PaletteScene_AdvanceTransition(void);
void PaletteScene_AdvanceOrbit();
void PaletteScene_SetRecordValue(s32 key, s32 value);
void PaletteScene_AdjustPaletteWindow(s32 adjustment);
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment);

#endif
