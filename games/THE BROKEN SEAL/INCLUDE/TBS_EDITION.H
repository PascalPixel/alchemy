#ifndef GUARD_TBS_EDITION_H
#define GUARD_TBS_EDITION_H

/* The Japanese fortress passage returns through the ninth window variant. */
#if defined(TBS_EDITION_JA)
#define RUNPA_RETURN_TRANSITION_VARIANT 9
#else
#define RUNPA_RETURN_TRANSITION_VARIANT 0
#endif

/* The Japanese render work carries 0x90 more bytes before these flags:
   four more 0x24-byte window records, twelve against eight. */
#if defined(TBS_EDITION_JA)
#define RENDER_DIRTY_OFS 0xF33
#define RENDER_MODE_OFS  0xF34
#define RENDER_BUSY_OFS  0x1189
#define RENDER_VALUE_TBL_OFS 0x114C
#define RENDER_NAME_TBL_OFS  0x116C
#define RENDER_CHANNEL_OFS   0x6B0
#define RENDER_OUTPUT_TBL_OFS 0x728
#define RENDER_ENTRY_TBL_OFS 0xF40
#define RENDER_MENU_STATE_OFS 0xF35
#define RENDER_MENU_BUSY_OFS  0xF36
#define RENDER_ALT_OFS        0xF32
#define RENDER_WORD_OFS       0xF3C
#define RENDER_WORD2_OFS      0xF38
#define RENDER_PARAM_OFS      0xF3E
#define RENDER_TILE_ATTR_OFS  0xE30
#define RENDER_FREE_HEAD_OFS  0xE28
#define RENDER_FREE_TAIL_OFS  0xE2C
#define RENDER_ENTRY_COUNT_OFS 0x1142
#define RENDER_RESULT_OFS      0x1184
#define RENDER_ENTRY_MASK      0xFF
#define WINDOW_COUNT          12
#define RENDER_LEVEL_OFS      0xF37
#define RENDER_COUNTER_OFS    0x1146
#define RENDER_WORK_SIZE      0x118C
#define RENDER_PALETTE_TBL_OFS 0x1160
#define RENDER_SLOT_ID_OFS    0x117C
#define RENDER_SLOT_VALUE_OFS 0x1180
#define SELECT_MENU_WIDTH    5
#define TYPE_MENU_WIDTH      4
#define MENU_TEXT_X          100
#define MENU_LABEL_X         110
#define LIST_MARKER_CHAR     65
#else
#define RENDER_DIRTY_OFS 0xEA3
#define RENDER_MODE_OFS  0xEA4
#define RENDER_BUSY_OFS  0x12F9
#define RENDER_VALUE_TBL_OFS 0x12BC
#define RENDER_NAME_TBL_OFS  0x12DC
#define RENDER_CHANNEL_OFS   0x620
#define RENDER_OUTPUT_TBL_OFS 0x698
#define RENDER_ENTRY_TBL_OFS 0xEB0
#define RENDER_MENU_STATE_OFS 0xEA5
#define RENDER_MENU_BUSY_OFS  0xEA6
#define RENDER_ALT_OFS        0xEA2
#define RENDER_WORD_OFS       0xEAC
#define RENDER_WORD2_OFS      0xEA8
#define RENDER_PARAM_OFS      0xEAE
#define RENDER_TILE_ATTR_OFS  0xDA0
#define RENDER_FREE_HEAD_OFS  0xD98
#define RENDER_FREE_TAIL_OFS  0xD9C
#define RENDER_ENTRY_COUNT_OFS 0x12B2
#define RENDER_RESULT_OFS      0x12F4
#define RENDER_ENTRY_MASK      0x1FF
#define WINDOW_COUNT          8
#define RENDER_LEVEL_OFS      0xEA7
#define RENDER_COUNTER_OFS    0x12B6
#define RENDER_WORK_SIZE      0x12FC
#define RENDER_PALETTE_TBL_OFS 0x12D0
#define RENDER_SLOT_ID_OFS    0x12EC
#define RENDER_SLOT_VALUE_OFS 0x12F0
#define SELECT_MENU_WIDTH    7
#define TYPE_MENU_WIDTH      7
#define MENU_TEXT_X          74
#define MENU_LABEL_X         82
#define LIST_MARKER_CHAR     49
#endif

#define MENU_TEXT_Y 60

/* The localized lettering sheet has one more sixteen-colour palette. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define KOROSSEO_PALETTE_BYTES 192
#define LOBBY_PREFIX_LIMIT 7
#else
#define KOROSSEO_PALETTE_BYTES 160
#define LOBBY_PREFIX_LIMIT 4
#endif

/* The localized ticket windows leave another column for the coin count. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define TICKET_COUNTER_WINDOW_X 18
#define TICKET_COUNTER_WIDTH    12
#define TICKET_COUNTER_X        32
#else
#define TICKET_COUNTER_WINDOW_X 19
#define TICKET_COUNTER_WIDTH    11
#if defined(TBS_EDITION_JA)
#define TICKET_COUNTER_X        0
#else
#define TICKET_COUNTER_X        24
#endif
#endif

/* The shops' item list window, where the character-change and switch help
   start, the rows a menu page holds and the rows of character positions: each
   edition sizes them to its words. */
#if defined(TBS_EDITION_JA)
#define SHOP_LIST_X      16
#define SHOP_LIST_WIDTH  14
#define HELP_TEXT_X      16
#define SWITCH_HELP_X    64
#define PAGE_ROWS        6
#define MENU_ROW_COUNT   4
#elif defined(TBS_EDITION_DE)
#define SHOP_LIST_X      14
#define SHOP_LIST_WIDTH  16
#define HELP_TEXT_X      0
#define SWITCH_HELP_X    80
#define PAGE_ROWS        5
#define MENU_ROW_COUNT   8
#else
#define SHOP_LIST_X      15
#define SHOP_LIST_WIDTH  15
#define HELP_TEXT_X      0
#define SWITCH_HELP_X    80
#define PAGE_ROWS        5
#define MENU_ROW_COUNT   8
#endif

/* The Djinni joining window: the Italian line takes two more columns, so its
   window starts a column further left. */
#if defined(TBS_EDITION_IT)
#define DJINN_JOIN_X      1
#define DJINN_JOIN_WIDTH  28
#else
#define DJINN_JOIN_X      2
#define DJINN_JOIN_WIDTH  26
#endif

/* The options screen's windows, sliders, frame icons and setting names: the
   Japanese labels are narrower, so its windows are narrower and a column
   further right, the sliders start further left, the first row of frames
   sits further right and the settings a column further right; the German and
   French speed setting needs a wider slot. */
#if defined(TBS_EDITION_JA)
#define OPTION_WINDOW_X     2
#define OPTION_WINDOW_WIDTH 26
#define OPTION_SLIDER_X     100
#define OPTION_FRAME_X      92
#define OPTION_SETTING_X    168
#define OPTION_SETTING_END  192
#define OPTION_SPEED_END    192
#else
#define OPTION_WINDOW_X     1
#define OPTION_WINDOW_WIDTH 28
#define OPTION_SLIDER_X     140
#define OPTION_FRAME_X      84
#define OPTION_SETTING_X    160
#define OPTION_SETTING_END  184
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define OPTION_SPEED_END    208
#else
#define OPTION_SPEED_END    200
#endif
#endif

/* Where the item menu's in-stock label follows a member's count. */
#if defined(TBS_EDITION_DE)
#define STOCK_LABEL_X 32
#elif defined(TBS_EDITION_ES)
#define STOCK_LABEL_X 30
#elif defined(TBS_EDITION_FR)
#define STOCK_LABEL_X 29
#else
#define STOCK_LABEL_X 24
#endif

/* The reel game's falling title: the Spanish word has nine letters and
   starts further left. */
#if defined(TBS_EDITION_ES)
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_X       8
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_X       24
#endif

#if defined(TBS_EDITION_JA)
#define RENDER_RESOURCE_BASE 0x9D4
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
      defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define RENDER_RESOURCE_BASE 0xBDF
#else
#define RENDER_RESOURCE_BASE 0x99B
#endif

/* The Psynergy list's range tiles, six Japanese rows and text columns. */
#if defined(TBS_EDITION_JA)
#define RANGE_ICON_TILE       0xf200
#define LIST_FIRST_ROW        1
#define LIST_ICONS_X          86
#define LIST_ICONS_Y          53
#define LIST_PP_X             144
#define LIST_NAME_X           96
#define LIST_PP_NUMBER_X      168
#define LIST_EMPTY_Y          8
#else
#define RANGE_ICON_TILE       0xf280
#define LIST_FIRST_ROW        2
#define LIST_ICONS_X          80
#define LIST_ICONS_Y          58
#define LIST_PP_X             176
#define LIST_NAME_X           88
#define LIST_PP_NUMBER_X      176
#define LIST_EMPTY_Y          17
#endif

#endif
