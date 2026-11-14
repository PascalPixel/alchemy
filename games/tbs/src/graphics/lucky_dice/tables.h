/// @file
/// The Lucky Dice minigame's tables, after LuckyDice_Run in both games.

// A table of signs nothing reads.
#include "edition.h"
const s32 gLuckyDice_UnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

/// How many sprites each kind of result's lettering takes: 32 pixels each,
/// for the kinds of lucky_dice.c's resultKind, 0 all four numbers alike, 1
/// three alike, 2 two pairs and 3 one pair.
const u8 gLuckyDice_LabelCounts[] = {5, 4, 3, 3};

/// The first tile of each kind of result's lettering.
const u8 gLuckyDice_LabelTiles[] = {
    28,  48,  64, 76,
#if (LANGUAGE != LANGUAGE_JAPANESE)
    28,  248, 64,
    248, // never read
#endif
};

/// The die face tile at each step of its roll: one for each 1024 of the roll
/// angle (variant), three to a face.
const u8 gLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                                   3, 9,  13, 4, 7,  6, 5, 17, 11};

/// The board's rows by depth: a die at rest is in the first row whose value its
/// z (16.16 pixels) exceeds, -32767 taking the rest.
const s16 gLuckyDice_FaceHeights[] = {147, 80, 15, -42, -95, -32767};
