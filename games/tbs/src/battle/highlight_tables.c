// The highlighted units' lightening.

#include "battle/battle.h"

// BLDY values, the same in both games.
Sint8 BattleHighlightBlendRamp[] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    4,  6,  7,  8,  8,  8,  8, 8, 4, 2, 1, 0, 0, 0, 0, 0,
};

// Unused: two words after the ramp that no code names.
Sint32 BattleHighlightUnusedWords[] = {1, 27};
