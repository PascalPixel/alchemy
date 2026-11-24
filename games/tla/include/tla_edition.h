#ifndef ALCHEMY_TLA_EDITION_H
#define ALCHEMY_TLA_EDITION_H

/// @file
/// Values that differ by language, chosen once for their code.

#include "edition.h"

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
#define FIELD_MENU_ICON_ROW_WIDTH 7
#define FILE_SCREEN_ICON_ROW_WIDTH 7
#else
#define FIELD_MENU_ICON_ROW_WIDTH 5
#define FILE_SCREEN_ICON_ROW_WIDTH 4
#endif

/// The debug scene selector's colons, where its arrow points, and the y of its
/// first row; further right in Japanese.
#if LANGUAGE != LANGUAGE_JAPANESE
#define DEBUG_SCENE_SELECTOR_COLON_X 74
#else
#define DEBUG_SCENE_SELECTOR_COLON_X 100
#endif
#define DEBUG_SCENE_SELECTOR_FIRST_ROW_Y 60

/// A link battle's opponent names take up to 8 letters of prefix, then a space,
/// in the European editions, instead of 5.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define BATTLE_ARENA_PREFIX_LAST 7
#else
#define BATTLE_ARENA_PREFIX_LAST 4
#endif

/// The first of the letters that tell enemies of one kind apart in battle, put
/// right after the name: the alphabet from 'A' in Japanese, the digits from '1'
/// in the others ("Chestbeater1" to "Chestbeater3").
#if LANGUAGE == LANGUAGE_JAPANESE
#define ENEMY_NAME_LETTER_FIRST 'A'
#else
#define ENEMY_NAME_LETTER_FIRST '1'
#endif

/// The letters of Lucky Wheels' title.
#if LANGUAGE == LANGUAGE_SPANISH
#define LUCKY_WHEELS_TITLE_LETTERS 9
#else
#define LUCKY_WHEELS_TITLE_LETTERS 8
#endif

#endif
