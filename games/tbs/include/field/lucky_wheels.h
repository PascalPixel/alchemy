#ifndef ALCHEMY_LUCKY_WHEELS_H
#define ALCHEMY_LUCKY_WHEELS_H

/// @file
/// Lucky Wheels, Tolbi's game of chance (lucky_wheels.c), and the unused LZ
/// packer kept beside it.

#include "fixed_math.h"
#include "types.h"

/// What a line of each Lucky Wheels symbol wins, as the prize sign reads it:
/// "Shirts win undershirts, Rings win rings, Boots win boots, Stars and Hearts
/// win items, and Moons can win you anything." Tolbi's attendant gives one of
/// three items of the kind.
enum LuckyWheelsPrize {
  LUCKY_WHEELS_PRIZE_UNDERSHIRT, ///< a Mythril, Silk or Running Shirt
  LUCKY_WHEELS_PRIZE_BOOTS,      ///< Hyper, Quick or Fur Boots
  LUCKY_WHEELS_PRIZE_RING,       ///< an Adept, War or Sleep Ring
  LUCKY_WHEELS_PRIZE_HEART_ITEM, ///< a Potion, Vial or Nut
  LUCKY_WHEELS_PRIZE_STAR_ITEM,  ///< a Water of Life, Psy Crystal or Elixir
  LUCKY_WHEELS_PRIZE_ANYTHING,   ///< one of the five kinds above
  /// Ends the list of prizes; first in it, the last pull won nothing.
  LUCKY_WHEELS_PRIZE_LIST_END = -1,
  /// First in the list: the player left at the bet, or Tolbi's attendant has
  /// given the prizes.
  LUCKY_WHEELS_PRIZE_LIST_DONE = -2
};

s32 LuckyWheels_Start(void);
void LuckyWheels_TransferCanvas(void);
void LuckyWheels_DarkenStep(void);
void LuckyWheels_StepSparkColors(s32 resource);
void LuckyWheels_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
extern const u16 gLuckyWheels_SparkPictures[];
extern const u8 gLuckyWheels_SymbolPrizes[];
extern const u8 gLuckyWheels_MachineSpriteX[];
extern const u8 gLuckyWheels_MachineSpriteY[];
void LuckyWheels_Update(void);
void LuckyWheels_SetUpBetting(void);
void LuckyWheels_Run(void);
void LzPack_ResetChains(void);
void LzPack_InsertSlotNode(s32 index);
void LzPack_RemoveSlotNode(s32 slot);
void LzPack_ReadInput(s32 start, s32 count, const u8 *input);
void LzPack_CopyBufferedBytes(u8 *destination);
s32 LzPack_PackData(const u8 *input, u8 *dst, s32 size);

#endif
