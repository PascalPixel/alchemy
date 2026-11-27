#ifndef ALCHEMY_TBS_EDITION_H
#define ALCHEMY_TBS_EDITION_H

// Values that differ by feature or language, chosen once for their code.

#include "edition.h"

// Lunpa Fortress's guard room fades in after its battle through a window transition: an ellipse
// around the leader in Japanese, random bars in the others.
#define LUNPA_RETURN_TRANSITION_VARIANT                                                            \
    WHEN(REVISED_ENTRANCE_TRANSITIONS, WINDOW_TRANSITION_BARS, WINDOW_TRANSITION_LEADER)

// Outside Japanese the windows' state (WIN_STATE) is laid out anew: 8 windows instead of 12, and a
// text ring of 512 codes instead of 256. The longer words widen the field menu's and the File
// Screen's icon rows and move the debug scene selector's columns, and two enemies of one kind are
// told apart in battle by 1 to 9 after their names instead of A to Z.
#if LANGUAGE == LANGUAGE_JAPANESE
#    define TEXT_BUFFER_MASK             0xFF
#    define WINDOW_COUNT                 12
#    define FIELD_MENU_ICON_ROW_WIDTH    5 // the field menu's icon row
#    define FILE_SCREEN_ICON_ROW_WIDTH   4 // the File Screen's icon row
// the debug scene selector's colons
#    define DEBUG_SCENE_SELECTOR_COLON_X 100
// its entrance number and area name
#    define DEBUG_SCENE_SELECTOR_LABEL_X 110
#    define ENEMY_NAME_LETTER_FIRST      'A'
#else
#    define TEXT_BUFFER_MASK             0x1FF
#    define WINDOW_COUNT                 8
#    define FIELD_MENU_ICON_ROW_WIDTH    7
#    define FILE_SCREEN_ICON_ROW_WIDTH   7
#    define DEBUG_SCENE_SELECTOR_COLON_X 74
#    define DEBUG_SCENE_SELECTOR_LABEL_X 82
#    define ENEMY_NAME_LETTER_FIRST      '1'
#endif

// The y of the debug scene selector's first row, in pixels, where its arrow points.
#define DEBUG_SCENE_SELECTOR_FIRST_ROW_Y 60

// The European editions' Colosso lettering keeps 192 bytes of palettes before its tiles instead of
// 160, and the Battle Arena's opponent names take up to 8 letters of prefix, then a space, instead
// of 5.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define COLOSSO_PALETTE_BYTES    192
#    define BATTLE_ARENA_PREFIX_LAST 7 // the index of the prefix's last letter
#else
#    define COLOSSO_PALETTE_BYTES    160
#    define BATTLE_ARENA_PREFIX_LAST 4
#endif

// The Kalay and Tolbi docks' ticket counter: its coins window, wider in the European editions, and
// where the number sits in it.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define DOCKS_COINS_WINDOW_X     18
#    define DOCKS_COINS_WINDOW_WIDTH 12
#    define DOCKS_COINS_X            32
#else
#    define DOCKS_COINS_WINDOW_X     19
#    define DOCKS_COINS_WINDOW_WIDTH 11
#    define DOCKS_COINS_X            JAPANESE_OR(0, 24)
#endif

// The menus' layout, which the localized editions widen or rearrange.
#if LANGUAGE == LANGUAGE_JAPANESE
#    define SHOP_SELL_PRICE_X          16 // the shop's sell and repair price window
#    define SHOP_SELL_PRICE_WIDTH      14 // its width, in tiles
// The item and Psynergy menus' L R: Change Char line (MsgChangeCharacterHelp).
#    define CHANGE_CHARACTER_HELP_X    16
#    define SWITCH_HELP_X              64 // the Status menu's help for switching members
#    define STATUS_LIST_PAGE_ROWS      6  // the Status menu's Psynergy names a page
// member figures the Status menu lowers
#    define STATUS_MEMBER_FIGURE_COUNT 4
#elif LANGUAGE == LANGUAGE_GERMAN
#    define SHOP_SELL_PRICE_X          14
#    define SHOP_SELL_PRICE_WIDTH      16
#    define CHANGE_CHARACTER_HELP_X    0
#    define SWITCH_HELP_X              80
#    define STATUS_LIST_PAGE_ROWS      5
#    define STATUS_MEMBER_FIGURE_COUNT 8
#else
#    define SHOP_SELL_PRICE_X          15
#    define SHOP_SELL_PRICE_WIDTH      15
#    define CHANGE_CHARACTER_HELP_X    0
#    define SWITCH_HELP_X              80
#    define STATUS_LIST_PAGE_ROWS      5
#    define STATUS_MEMBER_FIGURE_COUNT 8
#endif

// The shop's buying screen: the price window's width. The coins window's is SHOP_MONEY_WINDOW_WIDTH
// (shop_internal.h).
#define SHOP_PRICE_WIDTH BY_LANGUAGE(14, 15, 16, 15, 15, 15)

// The window that announces a Djinni joining, wider in Italian.
#define DJINN_JOIN_X     ITALIAN_OR(1, 2)
#define DJINN_JOIN_WIDTH ITALIAN_OR(28, 26)

// The options screen's layout: its window, sliders, icons and setting columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#    define OPTION_WINDOW_X     2
#    define OPTION_WINDOW_WIDTH 26
#    define OPTION_SLIDER_X     100
#    define OPTION_CHOICE_X     92
#    define OPTION_SETTING_X    168
#    define OPTION_SETTING_END  192
#    define OPTION_SPEED_END    192
#else
#    define OPTION_WINDOW_X     1
#    define OPTION_WINDOW_WIDTH 28
#    define OPTION_SLIDER_X     140
#    define OPTION_CHOICE_X     84
#    define OPTION_SETTING_X    160
#    define OPTION_SETTING_END  184
#    define OPTION_SPEED_END    BY_LANGUAGE(200, 200, 208, 200, 208, 200)
#endif

// Where the item menu puts its "in stock" label after the count.
#define STOCK_LABEL_X                  BY_LANGUAGE(24, 24, 32, 30, 29, 24)

// Lucky Wheels: its title's letters and where they start (nine in Spanish).
#define LUCKY_WHEELS_TITLE_LETTERS     SPANISH_OR(9, 8)
#define LUCKY_WHEELS_TITLE_X           SPANISH_OR(8, 24)

// Lucky Wheels' help window, and the VRAM its jackpot and title tiles use.
#define LUCKY_WHEELS_HELP_WINDOW_X     JAPANESE_OR(6, 2)
#define LUCKY_WHEELS_HELP_WINDOW_WIDTH JAPANESE_OR(18, 26)
#define LUCKY_WHEELS_JACKPOT_COPY      JAPANESE_OR(DMA_COPY_WORDS(1024), DMA_COPY_WORDS(1152))
#define LUCKY_WHEELS_TITLE_TILES       JAPANESE_OR(896, 880)
// The sprite tiles the jackpot's title letters are copied to.
#define LUCKY_WHEELS_JACKPOT_TILES     (OBJ_VRAM0 + LUCKY_WHEELS_TITLE_TILES * TILE_SIZE_4BPP)

// Lucky Wheels' prompt windows (stop, all held, start, hold, release, choose the reels to hold),
// placed and sized for each language's words. Japanese shows hold and release in one window.
#if LANGUAGE == LANGUAGE_JAPANESE
#    define LUCKY_WHEELS_STOP_X         23
#    define LUCKY_WHEELS_STOP_WIDTH     7
#    define LUCKY_WHEELS_ALL_HELD_X     12
#    define LUCKY_WHEELS_ALL_HELD_WIDTH 18
#    define LUCKY_WHEELS_START_X        18
#    define LUCKY_WHEELS_START_WIDTH    12
#    define LUCKY_WHEELS_HOLD_X         18
#    define LUCKY_WHEELS_HOLD_WIDTH     12
#elif LANGUAGE == LANGUAGE_SPANISH
#    define LUCKY_WHEELS_STOP_X         21
#    define LUCKY_WHEELS_STOP_WIDTH     9
#    define LUCKY_WHEELS_ALL_HELD_X     6
#    define LUCKY_WHEELS_ALL_HELD_WIDTH 24
#    define LUCKY_WHEELS_START_X        22
#    define LUCKY_WHEELS_START_WIDTH    8
#    define LUCKY_WHEELS_HOLD_X         22
#    define LUCKY_WHEELS_HOLD_WIDTH     8
#    define LUCKY_WHEELS_RELEASE_X      22
#    define LUCKY_WHEELS_RELEASE_WIDTH  8
#elif LANGUAGE == LANGUAGE_ITALIAN
#    define LUCKY_WHEELS_STOP_X         18
#    define LUCKY_WHEELS_STOP_WIDTH     12
#    define LUCKY_WHEELS_ALL_HELD_X     10
#    define LUCKY_WHEELS_ALL_HELD_WIDTH 20
#    define LUCKY_WHEELS_START_X        21
#    define LUCKY_WHEELS_START_WIDTH    9
#    define LUCKY_WHEELS_HOLD_X         21
#    define LUCKY_WHEELS_HOLD_WIDTH     9
#    define LUCKY_WHEELS_RELEASE_X      21
#    define LUCKY_WHEELS_RELEASE_WIDTH  9
#elif LANGUAGE == LANGUAGE_GERMAN
#    define LUCKY_WHEELS_STOP_X         18
#    define LUCKY_WHEELS_STOP_WIDTH     12
#    define LUCKY_WHEELS_ALL_HELD_X     11
#    define LUCKY_WHEELS_ALL_HELD_WIDTH 19
#    define LUCKY_WHEELS_START_X        16
#    define LUCKY_WHEELS_START_WIDTH    14
#    define LUCKY_WHEELS_HOLD_X         22
#    define LUCKY_WHEELS_HOLD_WIDTH     8
#    define LUCKY_WHEELS_RELEASE_X      21
#    define LUCKY_WHEELS_RELEASE_WIDTH  9
#else
#    define LUCKY_WHEELS_STOP_X         18
#    define LUCKY_WHEELS_STOP_WIDTH     12
#    define LUCKY_WHEELS_ALL_HELD_X     11
#    define LUCKY_WHEELS_ALL_HELD_WIDTH 19
#    define LUCKY_WHEELS_START_X        16
#    define LUCKY_WHEELS_START_WIDTH    14
#    define LUCKY_WHEELS_HOLD_X         23
#    define LUCKY_WHEELS_HOLD_WIDTH     7
#    define LUCKY_WHEELS_RELEASE_X      23
#    define LUCKY_WHEELS_RELEASE_WIDTH  7
#endif
#define LUCKY_WHEELS_CHOOSE_HELD_X         JAPANESE_OR(7, 3)
#define LUCKY_WHEELS_CHOOSE_HELD_WIDTH     JAPANESE_OR(16, 24)
// Lucky Wheels' count of Game Tickets left after the bet (LUCKY_WHEELS_TICKET_DIGITS and _X) takes
// three digits from the French release on, one tile further left so that it ends where the two did,
// and the hold hint no longer follows the fourth and last spin, after which no wheel can be held.
// Neither is about words, so both are patches.
#define LUCKY_WHEELS_TICKET_DIGITS         WHEN(LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT, 3, 2)
#define LUCKY_WHEELS_TICKET_X              WHEN(LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT, 56, 64)
#define LUCKY_WHEELS_HINT_BEFORE_LAST_SPIN WHEN(LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT, 1, 0)

// The Status menu's Psynergy list: its range icon tiles, rows and columns.
#define RANGE_ICON_TILE                    JAPANESE_OR(WINDOW_TILE(512), WINDOW_TILE(640))
#define STATUS_PSYNERGY_LIST_FIRST_ROW     JAPANESE_OR(1, 2)
#define STATUS_PSYNERGY_LIST_ICONS_X       JAPANESE_OR(86, 80)
#define STATUS_PSYNERGY_LIST_ICONS_Y       JAPANESE_OR(53, 58)
#define STATUS_PSYNERGY_LIST_PP_X          JAPANESE_OR(144, 176)
#define STATUS_PSYNERGY_LIST_NAME_X        JAPANESE_OR(96, 88)
#define STATUS_PSYNERGY_LIST_PP_NUMBER_X   JAPANESE_OR(168, 176)
#define STATUS_PSYNERGY_LIST_EMPTY_Y       JAPANESE_OR(8, 17)

// Where the save screen's file list puts the leader's name.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH     \
    || LANGUAGE == LANGUAGE_ITALIAN
#    define FILE_LIST_NAME_X 6
#else
#    define FILE_LIST_NAME_X 12
#endif

#endif
