// The Lucky Dice minigame's tables, after LuckyDice_Run in both games.

// A table of signs nothing reads.
const s32 gLuckyDice_UnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

// How many sprites each kind of result's lettering takes.
const u8 gLuckyDice_LabelCounts[] = {5, 4, 3, 3};

// The first tile of each kind of result's lettering.
const u8 gLuckyDice_LabelTiles[] = {
    28,
    48,
    64,
    76,
#if EDITION_INTERNATIONAL
    28,
    248,
    64,
    248, // never read
#endif
};

// The die face tile at each step of its roll.
const u8 gLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10, 3, 9, 13, 4, 7, 6, 5, 17, 11};

// The heights that decide the face (z > value << 16).
const s16 gLuckyDice_FaceHeights[] = {147, 80, 15, -42, -95, -32767};
