#ifndef ALCHEMY_MODULE_OVERWORLD_LUCKY_WHEELS_H
#define ALCHEMY_MODULE_OVERWORLD_LUCKY_WHEELS_H

#include "types.h"

s32 LuckyWheels_Start(void);
s32 LuckyWheels_ScaleRgb555(u16 *src, u16 *dst, s32 scale, s32 count);
void LuckyWheels_TransferCanvas(void);
void Palette_DarkenScreenStep(void);
void Palette_StepTowardResource(s32 resource);
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
extern const u16 gLuckyWheels_SparkCellOffsets[];
extern const u8 gLuckyWheels_SymbolPrizes[];
extern const u8 gLuckyWheels_MachineSpriteX[];
extern const u8 gLuckyWheels_MachineSpriteY[];
void LuckyWheels_Update(void);
void LuckyWheels_InitializeTitle(void);
void LuckyWheels_Run(void);
void LzPack_ResetSlotBuckets(void);
void LzPack_InsertSlotNode(s32 index);
void LzPack_RemoveSlotNode(s32 slot);
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void LzPack_CopyBufferedBytes(u8 *destination);
s32 LzPack_PackStream(const u8 *input, u8 *dst, s32 size);

#endif
