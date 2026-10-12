#ifndef ALCHEMY_UNIT_BATTLE_ELEMFX_H
#define ALCHEMY_UNIT_BATTLE_ELEMFX_H

#include "types.h"

s32 BattleFx_UpdateStarField(void);
u32 Graphics_UploadVramBlock(void);
s32 BattleFx_InitializeStarField(s32 mode);
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
/* BattleFx_PlayUnitElementEffect is called before its definition with no prototype in scope. */
void BattlePres_RunWithZeroArguments(void);
void Runtime_RemoveIrqHandlerSlot2(void);

#endif
