#ifndef MINIGAME_H
#define MINIGAME_H

/* MINIGAME */

#include "types.h"
#include "edition.h"
#include "farcall.h"

extern const u8	 ReelGame_FrameSpriteX[];
extern const u8	 ReelGame_FrameSpriteY[];
extern const u16 ReelGame_SparkCellOffsets[];
extern const u8	 ReelGame_SymbolPrizes[];
extern const u8	 ReelGame_TitleLetterTiles[];
extern const u8	 ReelGame_TitleLetterWidths[];
void			 ReelGame_InitTitle(void);
void			 ReelGame_Run(void);
void			 ReelGame_RunFrame(void);

#endif
