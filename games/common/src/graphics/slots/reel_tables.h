// The reel game's tables, the same in both games. The includer defines
// REEL_TITLE_LETTERS, the letters of the title it spells.

// Where each spark cell (a square 2 to 20 pixels on a side) starts.
const u16 gReelGame_SparkCellOffsets[] = { 0, 4, 20, 56, 120, 220, 364, 560, 816, 1140 };

// The prize of each reel symbol.
const u8 gReelGame_SymbolPrizes[] = { 0, 3, 1, 4, 2, 5 };

// The tile of each title letter.
const u8 gReelGame_TitleLetterTiles[REEL_TITLE_LETTERS] = {
#if !EDITION_INTERNATIONAL
    0, 1, 2, 3, 4, 2, 5, 6
#elif defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
    7, 0, 1, 2, 3, 4, 5, 6, 7
#else
    0, 1, 2, 3, 4, 5, 6, 7
#endif
};

// The positions of the frame's sprites.
const u8 gReelGame_FrameSpriteX[] = { 2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219,
    219 };
const u8 gReelGame_FrameSpriteY[] = { 2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108 };

// The advance of each title letter.
const u8 gReelGame_TitleLetterWidths[REEL_TITLE_LETTERS] = {
#if !EDITION_INTERNATIONAL
    32, 24, 22, 28, 28, 22, 18
#elif defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
    16, 28, 28, 26, 26, 28, 28, 20, 24
#else
    28, 28, 26, 26, 28, 28, 20, 24
#endif
};
