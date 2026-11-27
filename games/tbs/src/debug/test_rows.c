// An unused table that the Menu test room and the Test rooms both keep, included at its place in
// each.

// Two halfwords in one word, the first in the low half.
#define HALFWORDS(low, high) ((high) << 16 | (low))

// Unused: eight rows of seven words, the same in both overlays. No other data in the ROM has their
// layout. The first and fifth words hold two halfwords each, and the fourth word is the third
// in 16.16.
// clang-format off
Uint32 TestUnusedRows[] = {
	HALFWORDS(80, 80),   0, 80,  toFIXED(80),  HALFWORDS(3, 1), 2, 0,
	HALFWORDS(120, 120), 0, 120, toFIXED(120), HALFWORDS(3, 1), 2, 0,
	HALFWORDS(60, 40),   0, 128, toFIXED(128), HALFWORDS(2, 1), 1, 0,
	HALFWORDS(60, 40),   0, 128, toFIXED(128), HALFWORDS(2, 1), 3, 0,
	HALFWORDS(128, 128), 0, 128, toFIXED(128), HALFWORDS(3, 2), 1, 0,
	HALFWORDS(128, 128), 0, 128, toFIXED(128), HALFWORDS(3, 2), 1, 0,
	HALFWORDS(32, 32),   0, 0,   0,            0,               0, 0,
	HALFWORDS(128, 128), 0, 128, toFIXED(128), HALFWORDS(3, 2), 1, 0,
};
// clang-format on

#undef HALFWORDS
