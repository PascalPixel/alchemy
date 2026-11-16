#ifndef GUARD_TLA_EDITION_H
#define GUARD_TLA_EDITION_H

#include "edition.h"

#define RENDER_CHANNEL_OFFSET 0x6B8
#define RENDER_DIRECT_OFFSET 0x508
#define WINDOW_COUNT 12

// The window text ring: 256 codes in Japanese, 512 in the others, as The Broken
// Seal's.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TEXT_BUFFER_MASK 0xFF
#else
#define TEXT_BUFFER_MASK 0x1FF
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
#define SELECT_MENU_WIDTH 7
#else
#define SELECT_MENU_WIDTH 5
#endif

// The save-slot action menu's width.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define SAVE_SLOT_MENU_WIDTH 7
#else
#define SAVE_SLOT_MENU_WIDTH 4
#endif

// Where the destination row's cursor text sits; further right in Japanese.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define MENU_TEXT_X 74
#else
#define MENU_TEXT_X 100
#endif
#define MENU_TEXT_Y 60

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define NAME_PREFIX_LAST_INDEX 7
#else
#define NAME_PREFIX_LAST_INDEX 4
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define SUMMON_CHANNEL_MARKER 'A'
#else
#define SUMMON_CHANNEL_MARKER '1'
#endif

// The letters of Lucky Wheels' title, as in The Broken Seal.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define LUCKY_WHEELS_TITLE_LETTERS 9
#else
#define LUCKY_WHEELS_TITLE_LETTERS 8
#endif

#endif
