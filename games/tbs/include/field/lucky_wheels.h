#ifndef ALCHEMY_LUCKY_WHEELS_H
#define ALCHEMY_LUCKY_WHEELS_H

/* @file */
/* Lucky Wheels, Tolbi's game of chance (lucky_wheels.c), and the unused LZ */
/* packer kept beside it. */

#include "fixed_math.h"
#include "types.h"

/* What a line of each Lucky Wheels symbol wins, as the prize sign reads it: */
/* "Shirts win undershirts, Rings win rings, Boots win boots, Stars and Hearts
 */
/* win items, and Moons can win you anything." Tolbi's attendant gives one of */
/* three items of the kind. */
typedef enum LUCKY_WHEELS_PRIZE {
	LUCKY_WHEELS_PRIZE_UNDERSHIRT, /* a Mythril, Silk or Running Shirt */
	LUCKY_WHEELS_PRIZE_BOOTS,	   /* Hyper, Quick or Fur Boots */
	LUCKY_WHEELS_PRIZE_RING,	   /* an Adept, War or Sleep Ring */
	LUCKY_WHEELS_PRIZE_HEART_ITEM, /* a Potion, Vial or Nut */
	LUCKY_WHEELS_PRIZE_STAR_ITEM,  /* a Water of Life, Psy Crystal or Elixir */
	LUCKY_WHEELS_PRIZE_ANYTHING,   /* one of the five kinds above */
	/* Ends the list of prizes; first in it, the last pull won nothing. */
	LUCKY_WHEELS_PRIZE_LIST_END = -1,
	/* First in the list: the player left at the bet, or Tolbi's attendant has
	 */
	/* given the prizes. */
	LUCKY_WHEELS_PRIZE_LIST_DONE = -2
} LUCKY_WHEELS_PRIZE;

/* The Lucky Wheels module's function in its stub table */
/* (src/field/lucky_wheels_stubs.s). */
s32 fdLuckyWheelsStart(void);

void fdLuckyWheelsTransferCanvas(void);
void fdLuckyWheelsDarkenStep(void);
void fdLuckyWheelsStepSparkCols(s32 resource);
void fdLuckyWheelsDrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
extern const u16 LuckyWheels_SparkPictures[];
extern const u8 LuckyWheels_SymbolPrizes[];
extern const u8 LuckyWheels_MachineSpriteX[];
extern const u8 LuckyWheels_MachineSpriteY[];
void fdLuckyWheelsUpdate(void);
void fdLuckyWheelsSetUpBetting(void);
void fdLuckyWheelsRun(void);
void fdLzPackResetChains(void);
void fdLzPackInsertSlotNode(s32 index);
void fdLzPackRemoveSlotNode(s32 slot);
void fdLzPackReadInput(s32 start, s32 count, const u8 *input);
void fdLzPackCopyBufferedBytes(u8 *destination);
s32 fdLzPackPackData(const u8 *input, u8 *dst, s32 size);

#endif
