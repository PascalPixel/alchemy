#ifndef ALCHEMY_LUCKY_WHEELS_H
#define ALCHEMY_LUCKY_WHEELS_H

/// @file
/// Lucky Wheels, Tolbi's game of chance (lucky_wheels.c), and the unused LZ
/// packer kept beside it.

#include "fixed_math.h"
#include "types.h"

s32 LuckyWheels_Start(void);
void LuckyWheels_TransferCanvas(void);
void Palette_DarkenScreenStep(void);
void Palette_StepTowardResource(s32 resource);
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
extern const u16 gLuckyWheels_SparkPictures[];
extern const u8 gLuckyWheels_SymbolPrizes[];
extern const u8 gLuckyWheels_MachineSpriteX[];
extern const u8 gLuckyWheels_MachineSpriteY[];
void LuckyWheels_Update(void);
void LuckyWheels_InitializeTitle(void);
void LuckyWheels_Run(void);
void LzPack_ResetChains(void);
void LzPack_InsertSlotNode(s32 index);
void LzPack_RemoveSlotNode(s32 slot);
void LzPack_ReadInput(s32 start, s32 count, const u8 *input);
void LzPack_CopyBufferedBytes(u8 *destination);
s32 LzPack_PackData(const u8 *input, u8 *dst, s32 size);

#endif
