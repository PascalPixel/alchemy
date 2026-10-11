#ifndef ALCHEMY_UNIT_FIELD_COMMON_CAMERA_CAMERA_H
#define ALCHEMY_UNIT_FIELD_COMMON_CAMERA_CAMERA_H

#include "types.h"

s32 Runtime_BlankDisplayAndRun(void);
void Camera_ResetSceneDefaults(void);
s32 FixedPoint_Multiply8_8(s32 arg0, s32 arg1);
s16 Math_ScaleByRatio(s16 arg0, s16 arg1);
s16 FixedPoint_Reciprocal(s16 value);
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count);
void LuckyDice_Run(void);

#endif
