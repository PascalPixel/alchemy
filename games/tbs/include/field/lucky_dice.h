#ifndef ALCHEMY_LUCKY_DICE_H
#define ALCHEMY_LUCKY_DICE_H

// Lucky Dice, the dice game in Tolbi (lucky_dice.c), and the small fixed-point helpers it keeps for
// its 3D camera.

#include "fixed_math.h"
#include "types.h"

// The Lucky Dice module's function in its stub table (src/field/lucky_dice_stubs.s).
Sint32              fdLuckyDiceStart(void);

void                fdCameraResetViewDefaults(void);
Sint32              fdFixedPointMultiplyQ8(Sint16 left, Sint16 right);
Sint16              fdFixedPointDivQ8(Sint16 numerator, Sint16 denominator);
Sint16              fdFixedPointReciprocal(Sint16 value);
extern const Sint32 LuckyDiceUnusedSigns[];
extern const Uint8  LuckyDiceLabelCounts[];
extern const Uint8  LuckyDiceLabelTiles[];
extern const Uint8  LuckyDiceFaceTiles[];
extern const Sint16 LuckyDiceFaceHeights[];
void                fdLuckyDiceRun(void);

#endif
