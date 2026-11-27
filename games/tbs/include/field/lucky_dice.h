#ifndef ALCHEMY_LUCKY_DICE_H
#define ALCHEMY_LUCKY_DICE_H

/* @file */
/* Lucky Dice, the dice game in Tolbi (lucky_dice.c), and the small */
/* fixed-point helpers it keeps for its 3D camera. */

#include "fixed_math.h"
#include "types.h"

/* The Lucky Dice module's function in its stub table */
/* (src/field/lucky_dice_stubs.s). */
s32 fdLuckyDiceStart(void);

void fdCameraResetViewDefaults(void);
s32 fdFixedPointMulQ8(s16 left, s16 right);
s16 fdFixedPointDivQ8(s16 numerator, s16 denominator);
s16 fdFixedPointReciprocal(s16 value);
extern const s32 LuckyDice_UnusedSigns[];
extern const u8 LuckyDice_LabelCounts[];
extern const u8 LuckyDice_LabelTiles[];
extern const u8 LuckyDice_FaceTiles[];
extern const s16 LuckyDice_FaceHeights[];
void fdLuckyDiceRun(void);

#endif
