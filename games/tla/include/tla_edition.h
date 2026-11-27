#ifndef ALCHEMY_TLA_EDITION_H
#define ALCHEMY_TLA_EDITION_H

/* Values that differ by language, chosen once for their code. */

#include "edition.h"

/* The window text ring: 256 codes in Japanese, 512 in the others. */
#define TEXT_BUFFER_MASK JAPANESE_OR(0xFF, 0x1FF)

/* Two icon menus' widths, wider outside Japanese: the field menu's (Psynergy,
   Djinn, Item, Status) and the File Screen's (New Game, Continue, Copy, Erase,
   and Battle and Update when they are offered). */
#define FIELD_MENU_ICON_ROW_WIDTH  JAPANESE_OR(5, 7)
#define FILE_SCREEN_ICON_ROW_WIDTH JAPANESE_OR(4, 7)

/* The debug scene selector's colons, where its arrow points, and the y of its
   first row; further right in Japanese. */
#define DEBUG_SCENE_SELECTOR_COLON_X	 JAPANESE_OR(100, 74)
#define DEBUG_SCENE_SELECTOR_FIRST_ROW_Y 60

/* A link battle's opponent names take up to 8 letters of prefix, then a space,
   in the European editions, instead of 5. */
#define BATTLE_ARENA_PREFIX_LAST WHEN(SPACED_ENEMY_NAME_PREFIX, 7, 4)

/* The first of the letters that tell enemies of one kind apart in battle, put
   right after the name: the alphabet from 'A' in Japanese, the digits from '1'
   in the others ("Chestbeater1" to "Chestbeater3"). */
#define ENEMY_NAME_LETTER_FIRST JAPANESE_OR('A', '1')

/* The letters of Lucky Wheels' title. */
#define LUCKY_WHEELS_TITLE_LETTERS SPANISH_OR(9, 8)

#endif
