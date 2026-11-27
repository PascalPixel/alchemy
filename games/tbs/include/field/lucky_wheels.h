#ifndef ALCHEMY_LUCKY_WHEELS_H
#define ALCHEMY_LUCKY_WHEELS_H

/* Lucky Wheels, Tolbi's game of chance (lucky_wheels.c), and the unused LZ
   packer kept beside it. */

#include "fixed_math.h"
#include "types.h"

/* What a line of each Lucky Wheels symbol wins, as the prize sign reads it:
   "Shirts win undershirts, Rings win rings, Boots win boots, Stars and Hearts
   win items, and Moons can win you anything." Tolbi's attendant gives one of
   three items of the kind. */
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
	   given the prizes. */
	LUCKY_WHEELS_PRIZE_LIST_DONE = -2
} LUCKY_WHEELS_PRIZE;

/* The Lucky Wheels module's function in its stub table
   (src/field/lucky_wheels_stubs.s). */
Sint32 fdLuckyWheelsStart(void);

void fdLuckyWheelsTransferCanvas(void);
void fdLuckyWheelsDarkenStep(void);
void fdLuckyWheelsStepSparkCols(Sint32 resource);
void fdLuckyWheelsDrawCanvasLine(
	Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 color);
extern const Uint16 LuckyWheels_SparkPictures[];
extern const Uint8	LuckyWheels_SymbolPrizes[];
extern const Uint8	LuckyWheels_MachineSpriteX[];
extern const Uint8	LuckyWheels_MachineSpriteY[];
void				fdLuckyWheelsUpdate(void);
void				fdLuckyWheelsSetUpBetting(void);
void				fdLuckyWheelsRun(void);
void				fdLZPackResetChains(void);
void				fdLZPackInsertSlotNode(Sint32 index);
void				fdLZPackRemoveSlotNode(Sint32 slot);
void   fdLZPackReadInput(Sint32 start, Sint32 count, const Uint8 *input);
void   fdLZPackCopyBufferedBytes(Uint8 *destination);
Sint32 fdLZPackPackData(const Uint8 *input, Uint8 *dst, Sint32 size);

#endif
