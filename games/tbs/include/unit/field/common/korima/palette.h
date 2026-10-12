#ifndef ALCHEMY_UNIT_FIELD_COMMON_KORIMA_PALETTE_H
#define ALCHEMY_UNIT_FIELD_COMMON_KORIMA_PALETTE_H

#include "types.h"

void SceneEffect_AdjustPaletteColors(s32 a);
u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj);
void KorimaPalette_SaveFirst(void);
void KorimaPalette_SaveSecond(void);
void KorimaPalette_Capture(void);
void KorimaPalette_Restore(s32 second);

#endif
