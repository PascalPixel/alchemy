#ifndef ALCHEMY_UNIT_OBJECT_COLORFX_H
#define ALCHEMY_UNIT_OBJECT_COLORFX_H

#include "types.h"

s32 ReelGame_Start(void);
s32 Graphics_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count);
void ReelGame_TransferCanvas(void);
void Palette_DarkenSceneStep(void);
void Palette_StepTowardResource(s32 resource);
void ReelGame_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
extern const u16 gReelGame_SparkCellOffsets[];
extern const u8 gReelGame_SymbolPrizes[];
extern const u8 gReelGame_FrameSpriteX[];
extern const u8 gReelGame_FrameSpriteY[];
void ReelGame_RunFrame(void);
void ReelGame_InitializeTitle(void);
void ReelGame_Run(void);
void LzPack_ResetSlotBuckets(void);
void LzPack_InsertSlotNode(s32 index);
void LzPack_RemoveSlotNode(s32 slot);
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void LzPack_CopyBufferedBytes(u8 *destination);
s32 LzPack_PackStream(const u8 *input, u8 *dst, s32 size);

#endif
