#ifndef ALCHEMY_UNIT_WINDOW_TEXTSPD_H
#define ALCHEMY_UNIT_WINDOW_TEXTSPD_H

#include "types.h"

extern const u8 UiText_SpeedDelays[3];
extern const u8 UiText_SpeedSteps[3];
extern const u8 UiText_SpeedCueGaps[3];

/* The German counted nouns of graphics/text/numwords.c come right after these tables and stay their own object: in tbs-de the speed tables start at an odd address, right after genders.o, and in one file with UiText_NumberWords, a word-aligned pointer table, the object would start at the next word, 3 bytes later. Split evidence for textspd|numwords. */

#endif
