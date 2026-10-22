#ifndef GUARD_TBS_EDITION_H
#define GUARD_TBS_EDITION_H

/// @file
/// Values that differ by patch level or language, chosen once for their code.

#include "version.h"

// Lunpa Fortress's guard room fades in after its battle through a window
// transition: the ellipse around the leader (WINDOW_TRANSITION_LEADER) in
// Japanese, random bars (WINDOW_TRANSITION_BARS) in the others.
#if !TBS_PATCH_1
#define LUNPA_RETURN_TRANSITION_VARIANT 9
#else
#define LUNPA_RETURN_TRANSITION_VARIANT 0
#endif

// The window renderer's work (struct UiRenderWork) is laid out anew outside
// Japanese: 8 windows instead of 12, and a text ring of 512 codes instead of
// 256. With it change the debug menus' columns, two icon menus' widths, and
// the first enemy name suffix's character code (65 in Japanese, which has 26
// suffixes; 49 in the others, which have 9).
#if LANGUAGE == LANGUAGE_JAPANESE
#define RENDER_DIRTY_OFFSET 0xF33
#define RENDER_MODE_OFFSET 0xF34
#define RENDER_BUSY_OFFSET 0x1189
#define RENDER_VALUE_TABLE_OFFSET 0x114C
#define RENDER_NAME_TABLE_OFFSET 0x116C
#define RENDER_CHANNEL_OFFSET 0x6B0
#define RENDER_OUTPUT_TABLE_OFFSET 0x728
#define RENDER_ENTRY_TABLE_OFFSET 0xF40
#define RENDER_MENU_STATE_OFFSET 0xF35
#define RENDER_MENU_BUSY_OFFSET 0xF36
#define RENDER_ALT_OFFSET 0xF32
#define RENDER_WORD_OFFSET 0xF3C
#define RENDER_WORD2_OFFSET 0xF38
#define RENDER_PARAMETER_OFFSET 0xF3E
#define RENDER_TILE_ATTR_OFFSET 0xE30
#define RENDER_FREE_HEAD_OFFSET 0xE28
#define RENDER_FREE_TAIL_OFFSET 0xE2C
#define RENDER_ENTRY_COUNT_OFFSET 0x1142
#define RENDER_RESULT_OFFSET 0x1184
#define RENDER_ENTRY_MASK 0xFF
#define WINDOW_COUNT 12
#define RENDER_LEVEL_OFFSET 0xF37
#define RENDER_COUNTER_OFFSET 0x1146
#define RENDER_WORK_SIZE 0x118C
#define RENDER_PALETTE_TABLE_OFFSET 0x1160
#define RENDER_SLOT_ID_OFFSET 0x117C
#define RENDER_SLOT_VALUE_OFFSET 0x1180
#define SELECT_MENU_WIDTH 5
#define TYPE_MENU_WIDTH 4
#define MENU_TEXT_X 100
#define MENU_LABEL_X 110
#define LIST_MARKER_CHAR 65
#else
#define RENDER_DIRTY_OFFSET 0xEA3
#define RENDER_MODE_OFFSET 0xEA4
#define RENDER_BUSY_OFFSET 0x12F9
#define RENDER_VALUE_TABLE_OFFSET 0x12BC
#define RENDER_NAME_TABLE_OFFSET 0x12DC
#define RENDER_CHANNEL_OFFSET 0x620
#define RENDER_OUTPUT_TABLE_OFFSET 0x698
#define RENDER_ENTRY_TABLE_OFFSET 0xEB0
#define RENDER_MENU_STATE_OFFSET 0xEA5
#define RENDER_MENU_BUSY_OFFSET 0xEA6
#define RENDER_ALT_OFFSET 0xEA2
#define RENDER_WORD_OFFSET 0xEAC
#define RENDER_WORD2_OFFSET 0xEA8
#define RENDER_PARAMETER_OFFSET 0xEAE
#define RENDER_TILE_ATTR_OFFSET 0xDA0
#define RENDER_FREE_HEAD_OFFSET 0xD98
#define RENDER_FREE_TAIL_OFFSET 0xD9C
#define RENDER_ENTRY_COUNT_OFFSET 0x12B2
#define RENDER_RESULT_OFFSET 0x12F4
#define RENDER_ENTRY_MASK 0x1FF
#define WINDOW_COUNT 8
#define RENDER_LEVEL_OFFSET 0xEA7
#define RENDER_COUNTER_OFFSET 0x12B6
#define RENDER_WORK_SIZE 0x12FC
#define RENDER_PALETTE_TABLE_OFFSET 0x12D0
#define RENDER_SLOT_ID_OFFSET 0x12EC
#define RENDER_SLOT_VALUE_OFFSET 0x12F0
#define SELECT_MENU_WIDTH 7
#define TYPE_MENU_WIDTH 7
#define MENU_TEXT_X 74
#define MENU_LABEL_X 82
#define LIST_MARKER_CHAR 49
#endif

#define MENU_TEXT_Y 60

// The European editions' Colosso lettering keeps 192 bytes of palettes
// before its tiles instead of 160, and the link lobby's opponent names take up
// to 8 letters of prefix, then a space, instead of 5.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define COLOSSO_PALETTE_BYTES 192
#define LOBBY_PREFIX_LIMIT 7
#else
#define COLOSSO_PALETTE_BYTES 160
#define LOBBY_PREFIX_LIMIT 4
#endif

// The Kalay and Tolbi docks' ticket counter: its coins window, wider in the
// European editions, and where the number sits in it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TICKET_COUNTER_WINDOW_X 18
#define TICKET_COUNTER_WIDTH 12
#define TICKET_COUNTER_X 32
#else
#define TICKET_COUNTER_WINDOW_X 19
#define TICKET_COUNTER_WIDTH 11
#if LANGUAGE == LANGUAGE_JAPANESE
#define TICKET_COUNTER_X 0
#else
#define TICKET_COUNTER_X 24
#endif
#endif

// The menus' layout: the shop's sell and repair price window, the help line
// under the item and Psynergy menus, and the Status menu's switch help, page
// rows and row count, which the localized editions widen or rearrange.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_LIST_X 16
#define SHOP_LIST_WIDTH 14
#define HELP_TEXT_X 16
#define SWITCH_HELP_X 64
#define PAGE_ROWS 6
#define MENU_ROW_COUNT 4
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define SHOP_LIST_X 14
#define SHOP_LIST_WIDTH 16
#define HELP_TEXT_X 0
#define SWITCH_HELP_X 80
#define PAGE_ROWS 5
#define MENU_ROW_COUNT 8
#else
#define SHOP_LIST_X 15
#define SHOP_LIST_WIDTH 15
#define HELP_TEXT_X 0
#define SWITCH_HELP_X 80
#define PAGE_ROWS 5
#define MENU_ROW_COUNT 8
#endif

// The shop's buying screen: the coins and price windows' widths.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_MONEY_WIDTH 11
#define SHOP_PRICE_WIDTH 14
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define SHOP_MONEY_WIDTH 12
#define SHOP_PRICE_WIDTH 16
#else
#define SHOP_MONEY_WIDTH 12
#define SHOP_PRICE_WIDTH 15
#endif

// The window that announces a Djinni joining, wider in Italian.
#if (LANGUAGE == LANGUAGE_ITALIAN)
#define DJINN_JOIN_X 1
#define DJINN_JOIN_WIDTH 28
#else
#define DJINN_JOIN_X 2
#define DJINN_JOIN_WIDTH 26
#endif

// The options screen's layout: its window, sliders, icons and setting columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define OPTION_WINDOW_X 2
#define OPTION_WINDOW_WIDTH 26
#define OPTION_SLIDER_X 100
#define OPTION_FRAME_X 92
#define OPTION_SETTING_X 168
#define OPTION_SETTING_END 192
#define OPTION_SPEED_END 192
#else
#define OPTION_WINDOW_X 1
#define OPTION_WINDOW_WIDTH 28
#define OPTION_SLIDER_X 140
#define OPTION_FRAME_X 84
#define OPTION_SETTING_X 160
#define OPTION_SETTING_END 184
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define OPTION_SPEED_END 208
#else
#define OPTION_SPEED_END 200
#endif
#endif

// Where the item menu puts its "in stock" label after the count.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define STOCK_LABEL_X 32
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define STOCK_LABEL_X 30
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define STOCK_LABEL_X 29
#else
#define STOCK_LABEL_X 24
#endif

// The reel game: its title's letters and where they start (nine in Spanish).
#if (LANGUAGE == LANGUAGE_SPANISH)
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_X 8
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_X 24
#endif

// The reel game's help window, and the VRAM its jackpot and title tiles use.
#if LANGUAGE != LANGUAGE_JAPANESE
#define REEL_HELP_WINDOW_X 2
#define REEL_HELP_WINDOW_WIDTH 26
#define REEL_JACKPOT_TILES 0x06016e00
#define REEL_JACKPOT_COPY 0x84000480
#define REEL_TITLE_TILES 0x370
#else
#define REEL_HELP_WINDOW_X 6
#define REEL_HELP_WINDOW_WIDTH 18
#define REEL_JACKPOT_TILES 0x06017000
#define REEL_JACKPOT_COPY 0x84000400
#define REEL_TITLE_TILES 0x380
#endif

// The reel game's prompt windows (stop, all held, start, hold, release),
// placed and sized for each language's words.
#if LANGUAGE == LANGUAGE_JAPANESE
#define REEL_STOP_X 23
#define REEL_STOP_WIDTH 7
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define REEL_STOP_X 21
#define REEL_STOP_WIDTH 9
#define REEL_ALL_HELD_X 6
#define REEL_ALL_HELD_WIDTH 24
#define REEL_START_X 22
#define REEL_START_WIDTH 8
#define REEL_HOLD_X 22
#define REEL_HOLD_WIDTH 8
#define REEL_RELEASE_X 22
#define REEL_RELEASE_WIDTH 8
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define REEL_STOP_X 18
#define REEL_STOP_WIDTH 12
#define REEL_ALL_HELD_X 10
#define REEL_ALL_HELD_WIDTH 20
#define REEL_START_X 21
#define REEL_START_WIDTH 9
#define REEL_HOLD_X 21
#define REEL_HOLD_WIDTH 9
#define REEL_RELEASE_X 21
#define REEL_RELEASE_WIDTH 9
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define REEL_STOP_X 18
#define REEL_STOP_WIDTH 12
#define REEL_ALL_HELD_X 11
#define REEL_ALL_HELD_WIDTH 19
#define REEL_START_X 16
#define REEL_START_WIDTH 14
#define REEL_HOLD_X 22
#define REEL_HOLD_WIDTH 8
#define REEL_RELEASE_X 21
#define REEL_RELEASE_WIDTH 9
#else
#define REEL_STOP_X 18
#define REEL_STOP_WIDTH 12
#define REEL_ALL_HELD_X 11
#define REEL_ALL_HELD_WIDTH 19
#define REEL_START_X 16
#define REEL_START_WIDTH 14
#define REEL_HOLD_X 23
#define REEL_HOLD_WIDTH 7
#define REEL_RELEASE_X 23
#define REEL_RELEASE_WIDTH 7
#endif
// The reel game's count of Game Tickets left after the bet takes three digits
// from the French release on, one tile further left so that it ends where the
// two did, and the hold hint no longer follows the fourth and last spin, after
// which no reel can be held. Neither is about words, so both are updates.
#if TBS_PATCH_3
#define REEL_COIN_DIGITS 3
#define REEL_COIN_X 56
#define REEL_HINT_BEFORE_LAST_SPIN 1
#else
#define REEL_COIN_DIGITS 2
#define REEL_COIN_X 64
#define REEL_HINT_BEFORE_LAST_SPIN 0
#endif

// The first area name message, for the place name shown on entering a new
// place (UiTimedNotice_Create).
#if LANGUAGE == LANGUAGE_JAPANESE
#define RENDER_RESOURCE_BASE 0x9D4
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||       \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define RENDER_RESOURCE_BASE 0xBDF
#else
#define RENDER_RESOURCE_BASE 0x99B
#endif

// The Status menu's Psynergy list: its range icon tiles, rows and columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define RANGE_ICON_TILE 0xf200
#define LIST_FIRST_ROW 1
#define LIST_ICONS_X 86
#define LIST_ICONS_Y 53
#define LIST_PP_X 144
#define LIST_NAME_X 96
#define LIST_PP_NUMBER_X 168
#define LIST_EMPTY_Y 8
#else
#define RANGE_ICON_TILE 0xf280
#define LIST_FIRST_ROW 2
#define LIST_ICONS_X 80
#define LIST_ICONS_Y 58
#define LIST_PP_X 176
#define LIST_NAME_X 88
#define LIST_PP_NUMBER_X 176
#define LIST_EMPTY_Y 17
#endif

// Where the save screen's file list puts the leader's name.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SAVE_SLOT_NAME_X 6
#else
#define SAVE_SLOT_NAME_X 12
#endif

#endif
