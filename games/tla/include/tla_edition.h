#ifndef ALCHEMY_TLA_EDITION_H
#define ALCHEMY_TLA_EDITION_H

/// @file
/// Values that differ by language, chosen once for their code.

#include "edition.h"

/// The window renderer's windows, 12 in every edition.
#define WINDOW_COUNT 12

/// The window text ring: 256 codes in Japanese, 512 in the others.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TEXT_BUFFER_MASK 0xFF
#else
#define TEXT_BUFFER_MASK 0x1FF
#endif

/// Two icon menus' widths, wider outside Japanese: the field menu's (Psynergy,
/// Djinn, Item, Status) and the File Screen's (New Game, Continue, Copy, Erase,
/// and Battle and Update when they are offered).
#if LANGUAGE != LANGUAGE_JAPANESE
#define SELECT_MENU_WIDTH 7
#define TYPE_MENU_WIDTH 7
#else
#define SELECT_MENU_WIDTH 5
#define TYPE_MENU_WIDTH 4
#endif

/// Where the debug destination list's cursor sits; further right in Japanese.
#if LANGUAGE != LANGUAGE_JAPANESE
#define MENU_TEXT_X 74
#else
#define MENU_TEXT_X 100
#endif
#define MENU_TEXT_Y 60

// A link battle's opponent names take up to 8 letters of prefix, then a space,
// in the European editions, instead of 5.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define LOBBY_PREFIX_LIMIT 7
#else
#define LOBBY_PREFIX_LIMIT 4
#endif

/// The first of the letters that tell enemies of one kind apart in battle: the
/// alphabet from 'A' in Japanese, the digits from '1' in the others.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LIST_MARKER_CHAR 'A'
#else
#define LIST_MARKER_CHAR '1'
#endif

/// The letters of Lucky Wheels' title.
#if LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_TITLE_LETTERS 9
#else
#define LUCKY_WHEELS_TITLE_LETTERS 8
#endif

#endif
