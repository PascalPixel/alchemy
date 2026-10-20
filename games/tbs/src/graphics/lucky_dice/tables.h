/// @file
/// The Lucky Dice minigame's tables, after LuckyDice_Run in both games.

// A table of signs nothing reads.
#include "edition.h"
const s32 gLuckyDice_UnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

/// How many sprites each kind of result's lettering takes: 32 pixels each,
/// for the kinds of lucky_dice.c's resultKind, 0 all four numbers alike, 1
/// three alike, 2 two pairs and 3 one pair.
const u8 gLuckyDice_LabelCounts[] = {5, 4, 3, 3};

/// The first tile of each kind of result's lettering. A sprite's tile is the
/// value * 8 + 152, then 32 more for each sprite across, so the four
/// letterings lie one after another. Outside Japanese four more bytes
/// follow, none of which ☀️ reads, resultKind being 0 to 3.
const u8 gLuckyDice_LabelTiles[] = {
    28,  48,  64, 76,
#if (LANGUAGE != LANGUAGE_JAPANESE)
    28,  248, 64,
    248, // never read
#endif
};

/// The die face tile at each step of its roll: one for each 0x400 of the
/// roll angle (variant), three to a face. A face's first step shows the face
/// itself (tile n for face n), the next two pictures between faces.
const u8 gLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                                   3, 9,  13, 4, 7,  6, 5, 17, 11};

/// The board's rows by depth: a die at rest is in the first row whose value
/// its z (16.16 pixels) exceeds, -32767 taking the rest. The row, not the
/// face, is what they decide: the die's second number (numbers[2] and [3] in
/// lucky_dice.c).
const s16 gLuckyDice_FaceHeights[] = {147, 80, 15, -42, -95, -32767};
