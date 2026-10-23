/// @file
/// The Lucky Wheels tables, the same in both games. The includer defines
/// LUCKY_WHEELS_TITLE_LETTERS, the letters of the title it spells.

/// Where each spark cell (a square 2 to 20 pixels on a side) starts: size s
/// (1 to 10) is a square 2s pixels a side, after the smaller squares' pixels.
/// A spark is drawn at size variant / 12 + 1, so it shrinks as its variant
/// counts down.
const u16 gLuckyWheels_SparkCellOffsets[] = {0,   4,   20,  56,  120,
                                             220, 364, 560, 816, 1140};

/// The prize of each wheel symbol, kept in gGameState.wonPrizes for each
/// winning line. Symbol 5 matches any other on a line, so a line is won by
/// the symbol it shows besides 5. A line of five 5s leaves the symbol at -1
/// and reads the byte before this table; every wheel holds three 5s
/// (object/colorfx.c), so such a line can happen.
const u8 gLuckyWheels_SymbolPrizes[] = {0, 3, 1, 4, 2, 5};

/// The tile of each title letter, the letter's place in its sheet: jackpot.png
/// holds `Jackpot!` and jackpot_ja.png `ジャックポト!`. Japanese uses its ッ
/// twice for `ジャックポット!`, and Spanish opens with the `!` drawn flipped
/// both ways for `¡Jackpot!` (object/colorfx.c).
const u8 gLuckyWheels_TitleLetterTiles[LUCKY_WHEELS_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0, 1, 2, 3, 4,
    2, 5, 6
#elif LANGUAGE == LANGUAGE_SPANISH
    7, 0, 1, 2, 3,
    4, 5, 6, 7
#else
    0, 1, 2, 3, 4,
    5, 6, 7
#endif
};

/// The positions of the frame's fourteen sprites, in screen pixels. Lucky
/// Wheels gives the first four tile 0x4e0 and the rest 0x4e8
/// (object/colorfx.c).
const u8 gLuckyWheels_FrameSpriteX[] = {2,   0,  232, 232, 32,  32,  200,
                                        200, 28, 28,  208, 208, 219, 219};
const u8 gLuckyWheels_FrameSpriteY[] = {2,   152, 0,   152, 8,   104, 8,
                                        104, 118, 140, 118, 140, 6,   108};

/// The advance of each title letter, in pixels, from LUCKY_WHEELS_TITLE_X. The
/// last letter's advance is never used; Japanese leaves it out (0).
const u8 gLuckyWheels_TitleLetterWidths[LUCKY_WHEELS_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    32, 24, 22, 28,
    28, 22, 18
#elif LANGUAGE == LANGUAGE_SPANISH
    16, 28, 28, 26, 26,
    28, 28, 20, 24
#else
    28, 28, 26, 26, 28,
    28, 20, 24
#endif
};
