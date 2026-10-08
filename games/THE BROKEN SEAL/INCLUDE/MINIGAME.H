#ifndef MINIGAME_H
#define MINIGAME_H

/* MINIGAME */

#include "TYPES.H"
#include "EDITION.H"

extern const u8 ReelGame_FrameSpriteX[];
extern const u8 ReelGame_FrameSpriteY[];
extern const u16 ReelGame_SparkCellOffsets[];
extern const u8 ReelGame_SymbolPrizes[];
extern const u8 ReelGame_TitleLetterTiles[];
extern const u8 ReelGame_TitleLetterWidths[];
s32 PartyInventory_CountItemFar(s32 item);
void ReelGame_InitTitle(void);
void ReelGame_Run(void);
void ReelGame_RunFrame(void);

#endif
