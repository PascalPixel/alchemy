#ifndef ALCHEMY_LUCKY_DICE_H
#define ALCHEMY_LUCKY_DICE_H

/// @file
/// Lucky Dice, the dice game in Tolbi (lucky_dice.c), and the small
/// fixed-point helpers it keeps for its 3D camera.

#include "fixed_math.h"
#include "types.h"

s32 LuckyDice_Start(void);
void Camera_ResetViewDefaults(void);
s32 FixedPoint_MulQ8(s16 left, s16 right);
s16 Math_ScaleByRatio(s16 numerator, s16 denominator);
s16 FixedPoint_Reciprocal(s16 value);
extern const s32 gLuckyDice_UnusedSigns[];
extern const u8 gLuckyDice_LabelCounts[];
extern const u8 gLuckyDice_LabelTiles[];
extern const u8 gLuckyDice_FaceTiles[];
extern const s16 gLuckyDice_FaceHeights[];
void LuckyDice_Run(void);

#endif
