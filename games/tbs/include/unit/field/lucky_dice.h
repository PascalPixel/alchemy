#ifndef ALCHEMY_UNIT_FIELD_LUCKY_DICE_H
#define ALCHEMY_UNIT_FIELD_LUCKY_DICE_H

#include "types.h"

s32 Runtime_BlankDisplayAndRun(void);
void Camera_ResetSceneDefaults(void);
s32 FixedPoint_Multiply8_8(s32 leftValue, s32 rightValue);
s16 Math_ScaleByRatio(s16 numerator, s16 denominator);
s16 FixedPoint_Reciprocal(s16 value);
extern const s32 gLuckyDice_UnusedSigns[];
extern const u8 gLuckyDice_LabelCounts[];
extern const u8 gLuckyDice_LabelTiles[];
extern const u8 gLuckyDice_FaceTiles[];
extern const s16 gLuckyDice_FaceHeights[];
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
                               s32 count);
void LuckyDice_Run(void);

#endif
