#ifndef GUARD_TLA_EDITION_H
#define GUARD_TLA_EDITION_H

#include "edition.h"

#define RENDER_CHANNEL_OFS 0x6B8
#define RENDER_DIRECT_OFS  0x508
#define WINDOW_COUNT       12

#if EDITION_INTERNATIONAL
#define SELECT_MENU_WIDTH 7
#else
#define SELECT_MENU_WIDTH 5
#endif

#if EDITION_INTERNATIONAL
#define MENU_TEXT_X 74
#define MENU_TEXT_Y 60
#endif

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define NAME_PREFIX_LAST_INDEX 7
#else
#define NAME_PREFIX_LAST_INDEX 4
#endif

#if !EDITION_INTERNATIONAL
#define WINDOW_BUSY_FLAGS_OFS 0x118A
#define SUMMON_CHANNEL_MARKER 'A'
#elif defined(TLA_EDITION_DE)
#define WINDOW_BUSY_FLAGS_OFS 0x138A
#define SUMMON_CHANNEL_MARKER '1'
#else
#define WINDOW_BUSY_FLAGS_OFS 0x138A
#define SUMMON_CHANNEL_MARKER '1'
#endif

// The letters of the reel game's title, as in The Broken Seal.
#if defined(TLA_EDITION_ES)
#define REEL_TITLE_LETTERS 9
#else
#define REEL_TITLE_LETTERS 8
#endif

#endif
