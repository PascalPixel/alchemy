#ifndef ALCHEMY_EDITION_H
#define ALCHEMY_EDITION_H

/* The edition being built: its language, and the features it turns on. The code
   asks for a feature by name, #if FEATURE for a block and WHEN(FEATURE, on,
   off) for a value, and picks what the language decides with JAPANESE_OR,
   BY_LANGUAGE and the like (features.h). Every difference between editions that
   is not the language itself is a feature, turned on here for the editions that
   have it; every repair of the original's bugs and undefined behaviour is a fix
   (fixes.h), off in every edition and turned on by BUGFIX or UBFIX. */

/* The build defines one TBS_EDITION_ name, as 1; none at all is Japanese. */
#if (defined(TBS_EDITION_JA) + defined(TBS_EDITION_EN) + \
	defined(TBS_EDITION_DE) + defined(TBS_EDITION_ES) + \
	defined(TBS_EDITION_FR) + defined(TBS_EDITION_IT)) > 1
#error Select exactly one TBS edition
#endif
#if (defined(TBS_EDITION_JA) && TBS_EDITION_JA != 1) || \
	(defined(TBS_EDITION_EN) && TBS_EDITION_EN != 1) || \
	(defined(TBS_EDITION_DE) && TBS_EDITION_DE != 1) || \
	(defined(TBS_EDITION_ES) && TBS_EDITION_ES != 1) || \
	(defined(TBS_EDITION_FR) && TBS_EDITION_FR != 1) || \
	(defined(TBS_EDITION_IT) && TBS_EDITION_IT != 1)
#error Edition definitions must be 1
#endif

/* The languages LANGUAGE may be. Text, fonts and the layout that fits them
   follow the language; everything else that differs is a feature. */
#define LANGUAGE_JAPANESE 0
#define LANGUAGE_ENGLISH  1
#define LANGUAGE_GERMAN	  2
#define LANGUAGE_SPANISH  3
#define LANGUAGE_FRENCH	  4
#define LANGUAGE_ITALIAN  5

/* The edition's language, from the TBS_EDITION_ the build defines. */
#if defined(TBS_EDITION_EN)
#define LANGUAGE LANGUAGE_ENGLISH
#elif defined(TBS_EDITION_DE)
#define LANGUAGE LANGUAGE_GERMAN
#elif defined(TBS_EDITION_ES)
#define LANGUAGE LANGUAGE_SPANISH
#elif defined(TBS_EDITION_FR)
#define LANGUAGE LANGUAGE_FRENCH
#elif defined(TBS_EDITION_IT)
#define LANGUAGE LANGUAGE_ITALIAN
#else
#define LANGUAGE LANGUAGE_JAPANESE
#endif

#include "features.h"
#include "fixes.h"

/* The features each edition turns on. The catalogue below says what each one
   does. */
#if defined(TBS_EDITION_JA)
/* The Japanese edition turns none on. */
#elif defined(TBS_EDITION_EN)
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE		   1
#define ATALANTA_TRAILS_LOWER							   1
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY		   1
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR		   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE	   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW   1
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER				   1
#define CREDITS_REVISED_FOR_LOCALIZATION				   1
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE				   1
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE				   1
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES			   1
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED				   1
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS				   1
#define DEBUG_LEVEL_EDITOR_SMALLER						   1
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50					   1
#define DEBUG_MENU_TEST_NO_START_COINS					   1
#define DEBUG_SHOP_MESSAGE_BROWSER						   1
#define DEBUG_SHOP_SELLS_WEAPONS						   1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			   1
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE			   1
#define DRAGON_STATUE_LOGS_STAY_PUT						   1
#define EIGHT_PARTY_FIGURES_RESET						   1
#define EXPERIENCE_NEEDED_64_BIT						   1
#define FILE_LIST_GLOW_BAR_CURSOR						   1
#define GIFT_DROP_SKIPS_FULL_STACK_CHECK				   1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			   1
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT			   1
#define INNER_RELIEFS_SILENT							   1
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER					   1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					   1
#define ITEM_MENU_EXTRA_TEXT_TILES						   1
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION				   1
#define JUMP_LANDS_ON_CELL_CENTER						   1
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING			   1
#define LARGER_PASSWORD_BUFFER							   1
#define LUCKY_DICE_HELP_AFTER_LOSS						   1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					   1
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 1
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT  1
#define MOGALL_THIRD_AREA_EAST_APE_PLACED_UNTIL_OUT		   1
#define NUT_ROOM_JET_CARRIES_FURTHER					   1
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW					   1
#define PAUSE_HEADING_LOWER_IN_DEBUG					   1
#define RELIEF_WALL_SINGLE_TRIGGER						   1
#define REVEAL_RECAST_STAYS_ACTIVE						   1
#define REVISED_ENTRANCE_TRANSITIONS					   1
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT			   1
#define STATUS_MENU_EXTRA_TEXT_TILES					   1
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH   1
#define THREE_GUARDS_WEST_FACES_LEADER					   1
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH			   1
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS				   1
#define TRANSITION_MASK_COUNTS_SIDE_TEES				   1
#define UNUSED_BUSY_WAIT								   1
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED			   1
#define VALE_ROOF_MENDED_LEADER_GROUNDED				   1
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE	   1
#define VAULT_CHASE_MENU_STOPS_LEADER					   1
#define VAULT_SAP_STAYS_AFTER_BATTLE					   1
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS		   1
#define WINDOW_IS_CLOSED_NO_NULL_CHECK					   1
#define WINDOW_STATE_DMA_FILL							   1
#define WORLD_MAP_FIVE_PROBE_STEPS						   1
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY			   1
#define ZERO_WIDTH_GLYPH_ADVANCES						   1
#elif defined(TBS_EDITION_DE)
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE		   1
#define ATALANTA_TRAILS_LOWER							   1
#define BABI_PILLARS_MATCH_LEADER_PRIORITY				   1
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY		   1
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR		   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE	   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW   1
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER				   1
#define CREDITS_REVISED_FOR_LOCALIZATION				   1
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE				   1
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE				   1
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES			   1
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED				   1
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS				   1
#define DEBUG_LEVEL_EDITOR_SMALLER						   1
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50					   1
#define DEBUG_MENU_TEST_NO_START_COINS					   1
#define DEBUG_SECOND_FELIX_MESSAGE_TEST					   1
#define DEBUG_SHOP_MESSAGE_BROWSER						   1
#define DEBUG_SHOP_SELLS_WEAPONS						   1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			   1
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE			   1
#define DRAGON_STATUE_LOGS_STAY_PUT						   1
#define EIGHT_PARTY_FIGURES_RESET						   1
#define EXPERIENCE_NEEDED_64_BIT						   1
#define FIELD_MENU_STOPS_REVEAL_SPARKLES				   1
#define FILE_LIST_GLOW_BAR_CURSOR						   1
#define GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON			   1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			   1
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT			   1
#define HALT_ON_CARTRIDGE_REMOVED						   1
#define IODEM_DOOR_WALK_RESET							   1
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER					   1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					   1
#define ITEM_MENU_EXTRA_TEXT_TILES						   1
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION				   1
#define IWRAM_GAP_AFTER_KEYS_DIRECTION					   1
#define IWRAM_LAYOUT_SHIFT								   1
#define JET_ROOM_RESTORED_AFTER_STATUE_FALL				   1
#define JUMP_LANDS_ON_CELL_CENTER						   1
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING			   1
#define LARGER_PASSWORD_BUFFER							   1
#define LUCKY_DICE_HELP_AFTER_LOSS						   1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					   1
#define MAP_SCREEN_CURSOR_SNAPS							   1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			   1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			   1
#define MIND_READ_TEXT_NOT_INDENTED						   1
#define MIND_READ_WINDOW_NOT_WIDENED					   1
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 1
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT  1
#define MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY	   1
#define NUT_ROOM_JET_CARRIES_FURTHER					   1
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW					   1
#define PAUSE_HEADING_LOWER_IN_DEBUG					   1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					   1
#define RELIEF_WALL_SINGLE_TRIGGER						   1
#define REVISED_ENTRANCE_TRANSITIONS					   1
#define SPACED_ENEMY_NAME_PREFIX						   1
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT			   1
#define STATUS_MENU_EXTRA_TEXT_TILES					   1
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH   1
#define THREE_GUARDS_WEST_FACES_LEADER					   1
#define TITLE_GLOBALS_LEAD_SHORTER						   1
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH			   1
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS				   1
#define TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP				   1
#define TRANSITION_MASK_COUNTS_SIDE_TEES				   1
#define UNUSED_BUSY_WAIT								   1
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED			   1
#define VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH		   1
#define VALE_ROOF_MENDED_LEADER_GROUNDED				   1
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE	   1
#define VAULT_CHASE_MENU_STOPS_LEADER					   1
#define VAULT_SAP_STAYS_AFTER_BATTLE					   1
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS		   1
#define WIDE_STRING_FIXED_WIDTH							   1
#define WINDOW_IS_CLOSED_NO_NULL_CHECK					   1
#define WINDOW_STATE_DMA_FILL							   1
#define WORLD_MAP_FIVE_PROBE_STEPS						   1
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY			   1
#define ZERO_WIDTH_GLYPH_ADVANCES						   1
#elif defined(TBS_EDITION_ES)
#define ALTIN_COOK_LAST_POT_FURTHER_WEST				   1
#define ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED		   1
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE		   1
#define ATALANTA_TRAILS_LOWER							   1
#define BABI_FINALS_QUESTION							   1
#define BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST			   1
#define BABI_PILLARS_MATCH_LEADER_PRIORITY				   1
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY		   1
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR		   1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE	   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW   1
#define BOARD_SHIP_REMINDER_ENDS_EVENT					   1
#define BOARD_SHIP_REMINDER_ONLY_FROM_NORTH				   1
#define CARTRIDGE_LANGUAGE_AT_RUN_TIME					   1
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER				   1
#define CREDITS_REVISED_FOR_LOCALIZATION				   1
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE				   1
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE				   1
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES			   1
#define DEBUG_ABILITY_BROWSER_HIDDEN					   1
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED				   1
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS				   1
#define DEBUG_LEVEL_EDITOR_SMALLER						   1
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50					   1
#define DEBUG_MENU_TEST_NO_START_COINS					   1
#define DEBUG_SECOND_DORA_DANGER_MESSAGE				   1
#define DEBUG_SECOND_DORA_NUT_GIFT						   1
#define DEBUG_SECOND_FELIX_MESSAGE_TEST					   1
#define DEBUG_SHOP_MESSAGE_BROWSER						   1
#define DEBUG_SHOP_SELLS_WEAPONS						   1
#define DEBUG_STORY_PRESET_THROUGH_KALAY				   1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			   1
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE			   1
#define DRAGON_STATUE_LOGS_STAY_PUT						   1
#define EIGHT_PARTY_FIGURES_RESET						   1
#define EXPERIENCE_NEEDED_64_BIT						   1
#define FIELD_MENU_STOPS_REVEAL_SPARKLES				   1
#define FILE_LIST_GLOW_BAR_CURSOR						   1
#define GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON			   1
#define GIVE_TRADE_PROMPT_REPLACES_STOCK				   1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			   1
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT			   1
#define HALT_ON_CARTRIDGE_REMOVED						   1
#define IMIL_SNOWMAN_PLACED_ONLY_ON_ITS_CELL			   1
#define IODEM_DOOR_WALK_RESET							   1
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER					   1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					   1
#define ITEM_MENU_EXTRA_TEXT_TILES						   1
#define ITEM_QUANTITY_ADDS_ALL_STACKS					   1
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION				   1
#define JET_ROOM_RESTORED_AFTER_STATUE_FALL				   1
#define JUMP_LANDS_ON_CELL_CENTER						   1
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING			   1
#define KOLIMA_NUT_CHECKS_OWN_FLAG						   1
#define KRADENS_JAR_ELIXIR_AFTER_KALAY					   1
#define LARGER_PASSWORD_BUFFER							   1
#define LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR		   1
#define LUCKY_DICE_HELP_AFTER_LOSS						   1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					   1
#define LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT			   1
#define LUNPA_GATES_RELOAD_REAPPLIES_CLOAK				   1
#define LUNPA_REVEAL_KEEPS_ICE_PILLAR_COLLISION			   1
#define MANTICORE_MARKS_KALAY_REACHED					   1
#define MAP_SCREEN_CURSOR_SNAPS							   1
#define MCCOY_PALACE_GROUNDS_IVY						   1
#define MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED		   1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			   1
#define MIND_READ_TEXT_NOT_INDENTED						   1
#define MIND_READ_WINDOW_NOT_WIDENED					   1
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 1
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT  1
#define MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY	   1
#define NUT_ROOM_JET_CARRIES_FURTHER					   1
#define OUTLINED_GLYPH_PAIRS_FIT_CELL					   1
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW					   1
#define PAGE_BREAK_RESETS_STYLE							   1
#define PAUSE_HEADING_LOWER_IN_DEBUG					   1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					   1
#define RELIEF_WALL_SINGLE_TRIGGER						   1
#define RESISTANCE_DOWN_HELP_NO_MINUS					   1
#define REVISED_ENTRANCE_TRANSITIONS					   1
#define SENDING_WINDOW_WAITS_A_TICK_AFTER_CLOSING		   1
#define SET_DJINNI_WINDOW_FITS_SCREEN					   1
#define SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS			   1
#define SHORTER_ENDING_SHIP_DEPARTURE_DELAY				   1
#define SIDEWALK_DEMO_KEEPS_LEADER_EFFECT				   1
#define SOCKET_PILLAR_ENDS_REVEAL						   1
#define SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED			   1
#define SPACED_ENEMY_NAME_PREFIX						   1
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT			   1
#define STATUS_LABELS_FROM_ATTACK						   1
#define STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY		   1
#define STATUS_MENU_EXTRA_TEXT_TILES					   1
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH   1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS  1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION		   1
#define THREE_GUARDS_WEST_FACES_LEADER					   1
#define TITLE_GLOBALS_LEAD_SHORTER						   1
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH			   1
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS				   1
#define TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP				   1
#define TRADE_CANCEL_KEEPS_CHOSEN_ITEM					   1
#define TRANSFER_METHOD_MENU_UNCENTRED					   1
#define TRANSITION_MASK_COUNTS_SIDE_TEES				   1
#define UNUSED_BUSY_WAIT								   1
#define VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY			   1
#define VALE_DORA_SPOT_MATCHES_ROOF_SCENE				   1
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED			   1
#define VALE_PLAZA_SECOND_AIDE_MOVED_NORTHWEST			   1
#define VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED			   1
#define VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY		   1
#define VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH		   1
#define VALE_ROOF_MENDED_LEADER_GROUNDED				   1
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE	   1
#define VAULT_CHASE_MENU_STOPS_LEADER					   1
#define VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES		   1
#define VAULT_SAP_STAYS_AFTER_BATTLE					   1
#define VENUS_HOLE_DROP_HOLDS_LEADER					   1
#define VENUS_HOLE_DROP_OPENS_WIDER						   1
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS		   1
#define WIDE_STRING_FIXED_WIDTH							   1
#define WINDOW_IS_CLOSED_NO_NULL_CHECK					   1
#define WINDOW_STATE_DMA_FILL							   1
#define WORLD_MAP_FIVE_PROBE_STEPS						   1
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY			   1
#define WRECK_SEARCH_WITHOUT_FREEZE						   1
#define ZERO_WIDTH_GLYPH_ADVANCES						   1
#elif defined(TBS_EDITION_FR)
#define ALTIN_COOK_LAST_POT_FURTHER_WEST				   1
#define ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED		   1
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE		   1
#define ATALANTA_TRAILS_LOWER							   1
#define BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST			   1
#define BABI_PILLARS_MATCH_LEADER_PRIORITY				   1
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY		   1
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR		   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE	   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW   1
#define BOARD_SHIP_REMINDER_ENDS_EVENT					   1
#define BOARD_SHIP_REMINDER_ONLY_FROM_NORTH				   1
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER				   1
#define CREDITS_REVISED_FOR_LOCALIZATION				   1
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE				   1
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE				   1
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES			   1
#define DEBUG_ABILITY_BROWSER_HIDDEN					   1
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED				   1
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS				   1
#define DEBUG_LEVEL_EDITOR_SMALLER						   1
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50					   1
#define DEBUG_MENU_TEST_NO_START_COINS					   1
#define DEBUG_SECOND_DORA_NUT_GIFT						   1
#define DEBUG_SECOND_FELIX_MESSAGE_TEST					   1
#define DEBUG_SHOP_MESSAGE_BROWSER						   1
#define DEBUG_SHOP_SELLS_WEAPONS						   1
#define DEBUG_STORY_PRESET_THROUGH_KALAY				   1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			   1
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE			   1
#define DRAGON_STATUE_LOGS_STAY_PUT						   1
#define EIGHT_PARTY_FIGURES_RESET						   1
#define EXPERIENCE_NEEDED_64_BIT						   1
#define FIELD_MENU_STOPS_REVEAL_SPARKLES				   1
#define FILE_LIST_GLOW_BAR_CURSOR						   1
#define GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON			   1
#define GIVE_TRADE_PROMPT_REPLACES_STOCK				   1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			   1
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT			   1
#define HALT_ON_CARTRIDGE_REMOVED						   1
#define IMIL_SNOWMAN_PLACED_ONLY_ON_ITS_CELL			   1
#define IODEM_DOOR_WALK_RESET							   1
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER					   1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					   1
#define ITEM_MENU_EXTRA_TEXT_TILES						   1
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION				   1
#define JET_ROOM_RESTORED_AFTER_STATUE_FALL				   1
#define JUMP_LANDS_ON_CELL_CENTER						   1
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING			   1
#define KOLIMA_NUT_CHECKS_OWN_FLAG						   1
#define KRADENS_JAR_ELIXIR_AFTER_KALAY					   1
#define LARGER_PASSWORD_BUFFER							   1
#define LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR		   1
#define LUCKY_DICE_HELP_AFTER_LOSS						   1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					   1
#define LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT			   1
#define MANTICORE_MARKS_KALAY_REACHED					   1
#define MAP_SCREEN_CURSOR_SNAPS							   1
#define MCCOY_PALACE_GROUNDS_IVY						   1
#define MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED		   1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			   1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			   1
#define MIND_READ_TEXT_NOT_INDENTED						   1
#define MIND_READ_WINDOW_NOT_WIDENED					   1
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 1
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT  1
#define MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY	   1
#define NUT_ROOM_JET_CARRIES_FURTHER					   1
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW					   1
#define PAGE_BREAK_RESETS_STYLE							   1
#define PAUSE_HEADING_LOWER_IN_DEBUG					   1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					   1
#define RELIEF_WALL_SINGLE_TRIGGER						   1
#define REVISED_ENTRANCE_TRANSITIONS					   1
#define SET_DJINNI_WINDOW_FITS_SCREEN					   1
#define SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS			   1
#define SHORTER_ENDING_SHIP_DEPARTURE_DELAY				   1
#define SIDEWALK_DEMO_KEEPS_LEADER_EFFECT				   1
#define SOCKET_PILLAR_ENDS_REVEAL						   1
#define SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED			   1
#define SPACED_ENEMY_NAME_PREFIX						   1
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT			   1
#define STATUS_MENU_EXTRA_TEXT_TILES					   1
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH   1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS  1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION		   1
#define THREE_GUARDS_WEST_FACES_LEADER					   1
#define TITLE_GLOBALS_LEAD_SHORTER						   1
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH			   1
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS				   1
#define TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP				   1
#define TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW			   1
#define TRANSITION_MASK_COUNTS_SIDE_TEES				   1
#define UNUSED_BUSY_WAIT								   1
#define UNUSED_HP_PP_LABEL_TILES_DROPPED				   1
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED			   1
#define VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY		   1
#define VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH		   1
#define VALE_ROOF_MENDED_LEADER_GROUNDED				   1
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE	   1
#define VAULT_CHASE_MENU_STOPS_LEADER					   1
#define VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES		   1
#define VAULT_SAP_STAYS_AFTER_BATTLE					   1
#define VENUS_HOLE_DROP_HOLDS_LEADER					   1
#define VENUS_HOLE_DROP_OPENS_WIDER						   1
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS		   1
#define WIDE_STRING_FIXED_WIDTH							   1
#define WINDOW_IS_CLOSED_NO_NULL_CHECK					   1
#define WINDOW_STATE_DMA_FILL							   1
#define WORLD_MAP_FIVE_PROBE_STEPS						   1
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY			   1
#define WRECK_SEARCH_WITHOUT_FREEZE						   1
#define ZERO_WIDTH_GLYPH_ADVANCES						   1
#elif defined(TBS_EDITION_IT)
#define ALTIN_COOK_LAST_POT_FURTHER_WEST				   1
#define ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED		   1
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE		   1
#define ATALANTA_TRAILS_LOWER							   1
#define BABI_FINALS_QUESTION							   1
#define BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST			   1
#define BABI_PILLARS_MATCH_LEADER_PRIORITY				   1
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY		   1
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR		   1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE	   1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW   1
#define BOARD_SHIP_REMINDER_ENDS_EVENT					   1
#define BOARD_SHIP_REMINDER_ONLY_FROM_NORTH				   1
#define CARTRIDGE_LANGUAGE_AT_RUN_TIME					   1
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER				   1
#define CREDITS_REVISED_FOR_LOCALIZATION				   1
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE				   1
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE				   1
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES			   1
#define DEBUG_ABILITY_BROWSER_HIDDEN					   1
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED				   1
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS				   1
#define DEBUG_LEVEL_EDITOR_SMALLER						   1
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50					   1
#define DEBUG_MENU_TEST_NO_START_COINS					   1
#define DEBUG_SECOND_DORA_DANGER_MESSAGE				   1
#define DEBUG_SECOND_DORA_NUT_GIFT						   1
#define DEBUG_SECOND_FELIX_MESSAGE_TEST					   1
#define DEBUG_SHOP_MESSAGE_BROWSER						   1
#define DEBUG_SHOP_SELLS_WEAPONS						   1
#define DEBUG_STORY_PRESET_THROUGH_KALAY				   1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			   1
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE			   1
#define DRAGON_STATUE_LOGS_STAY_PUT						   1
#define EIGHT_PARTY_FIGURES_RESET						   1
#define EXPERIENCE_NEEDED_64_BIT						   1
#define FIELD_MENU_STOPS_REVEAL_SPARKLES				   1
#define FILE_LIST_GLOW_BAR_CURSOR						   1
#define GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON			   1
#define GIVE_TRADE_PROMPT_REPLACES_STOCK				   1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			   1
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT			   1
#define HALT_ON_CARTRIDGE_REMOVED						   1
#define IMIL_SNOWMAN_PLACED_ONLY_ON_ITS_CELL			   1
#define IODEM_DOOR_WALK_RESET							   1
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER					   1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					   1
#define ITEM_MENU_EXTRA_TEXT_TILES						   1
#define ITEM_QUANTITY_ADDS_ALL_STACKS					   1
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION				   1
#define JET_ROOM_RESTORED_AFTER_STATUE_FALL				   1
#define JUMP_LANDS_ON_CELL_CENTER						   1
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING			   1
#define KOLIMA_NUT_CHECKS_OWN_FLAG						   1
#define KRADENS_JAR_ELIXIR_AFTER_KALAY					   1
#define LARGER_PASSWORD_BUFFER							   1
#define LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR		   1
#define LUCKY_DICE_HELP_AFTER_LOSS						   1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					   1
#define LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT			   1
#define LUNPA_GATES_RELOAD_REAPPLIES_CLOAK				   1
#define LUNPA_REVEAL_KEEPS_ICE_PILLAR_COLLISION			   1
#define MANTICORE_MARKS_KALAY_REACHED					   1
#define MAP_SCREEN_CURSOR_SNAPS							   1
#define MCCOY_PALACE_GROUNDS_IVY						   1
#define MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED		   1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			   1
#define MIND_READ_TEXT_NOT_INDENTED						   1
#define MIND_READ_WINDOW_NOT_WIDENED					   1
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 1
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT  1
#define MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY	   1
#define NUT_ROOM_JET_CARRIES_FURTHER					   1
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW					   1
#define PAGE_BREAK_RESETS_STYLE							   1
#define PAUSE_HEADING_LOWER_IN_DEBUG					   1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					   1
#define RELIEF_WALL_SINGLE_TRIGGER						   1
#define RESISTANCE_DOWN_HELP_NO_MINUS					   1
#define REVISED_ENTRANCE_TRANSITIONS					   1
#define SENDING_WINDOW_WAITS_A_TICK_AFTER_CLOSING		   1
#define SET_DJINNI_WINDOW_FITS_SCREEN					   1
#define SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS			   1
#define SHORTER_ENDING_SHIP_DEPARTURE_DELAY				   1
#define SIDEWALK_DEMO_KEEPS_LEADER_EFFECT				   1
#define SOCKET_PILLAR_ENDS_REVEAL						   1
#define SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED			   1
#define SPACED_ENEMY_NAME_PREFIX						   1
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT			   1
#define STATUS_LABELS_FROM_ATTACK						   1
#define STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY		   1
#define STATUS_MENU_EXTRA_TEXT_TILES					   1
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH   1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS  1
#define SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION		   1
#define THREE_GUARDS_WEST_FACES_LEADER					   1
#define TITLE_GLOBALS_LEAD_SHORTER						   1
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH			   1
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS				   1
#define TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP				   1
#define TRADE_CANCEL_KEEPS_CHOSEN_ITEM					   1
#define TRANSITION_MASK_COUNTS_SIDE_TEES				   1
#define UNUSED_BUSY_WAIT								   1
#define VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY			   1
#define VALE_DORA_SPOT_MATCHES_ROOF_SCENE				   1
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED			   1
#define VALE_PLAZA_SECOND_AIDE_MOVED_NORTHWEST			   1
#define VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED			   1
#define VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY		   1
#define VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH		   1
#define VALE_ROOF_MENDED_LEADER_GROUNDED				   1
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE	   1
#define VAULT_CHASE_MENU_STOPS_LEADER					   1
#define VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES		   1
#define VAULT_SAP_STAYS_AFTER_BATTLE					   1
#define VENUS_HOLE_DROP_HOLDS_LEADER					   1
#define VENUS_HOLE_DROP_OPENS_WIDER						   1
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS		   1
#define WIDE_STRING_FIXED_WIDTH							   1
#define WINDOW_IS_CLOSED_NO_NULL_CHECK					   1
#define WINDOW_STATE_DMA_FILL							   1
#define WORLD_MAP_FIVE_PROBE_STEPS						   1
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY			   1
#define WRECK_SEARCH_WITHOUT_FREEZE						   1
#define ZERO_WIDTH_GLYPH_ADVANCES						   1
#endif

/* The features, each off unless an edition turns it on above. */

/* In Altin's inn, the cook from Xian stops at his last and easternmost pot at x
   380 instead of x 384, 4 pixels further west. */
#ifndef ALTIN_COOK_LAST_POT_FURTHER_WEST
#define ALTIN_COOK_LAST_POT_FURTHER_WEST 0
#endif

/* Three small climbing-exit boxes become 16-pixel cubes. These are both ends of
   Altin Peak's ladder between the falling-rocks passage and the Hydros statue's
   room, where two small boxes become one, and the vine exit from Tret's room.
   All other exit regions keep their sizes, including the other vine exits in
   the tree. */
#ifndef ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED
#define ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED 0
#endif

/* Dropping the stone pillar into the pit in Squall's hall sets a scene flag.
   When the hall reloads with the party kept in place (after a battle, or on
   continuing a suspended game), the walkable cell over the pit is copied again.
   Japanese loses the bridge on those reloads. */
#ifndef ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE
#define ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE 0
#endif

/* Atalanta's battle effect draws each of a gathering shaft's trail motes 16
   pixels below its projected point (ATALANTA_TRAIL_DROP is 16, not 0). Japanese
   draws the trails 16 pixels higher. Nothing else in the effect reads the
   value. */
#ifndef ATALANTA_TRAILS_LOWER
#define ATALANTA_TRAILS_LOWER 0
#endif

/* On finals day in Tolbi (Babi saved, Colosso not yet over), the strongman in
   the storeroom of Stucco Rooms B asks Isaac a yes/no question: was Babi's
   disappearance a misunderstanding? Yes and No each get their own answer. The
   Babi skeptic then says "Babi decided to show up for the finals?". In the
   other editions there is no question. The strongman only says the Yes answer
   as a fixed line, and the skeptic says the No answer as his own. */
#ifndef BABI_FINALS_QUESTION
#define BABI_FINALS_QUESTION 0
#endif

/* While Babi is missing, the attendant in Babi Palace's big bedroom offers the
   party the four beds and then walks back. Afterwards she is set exactly at her
   post, (56, 120), which is where the walk is meant to leave her. During the
   finals the same woman makes the same walk back (scBabiPalaceOfferRest), and
   that walk is not snapped in any edition. */
#ifndef BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST
#define BABI_PALACE_BED_ATTENDANT_SNAPS_TO_POST 0
#endif

/* At the Babi Lighthouse Entry, both pillars get an update function that runs
   every tick. A pillar north of the party leader takes his sprite priority. A
   pillar level with him or south of him is drawn one level in front of him,
   never below 1, so he passes behind it. In that south case the pillar is also
   left on automatic priority, so the camera can replace the value with the
   priority of the map cell under it. Without this feature both pillars stay at
   a fixed priority 1. */
#ifndef BABI_PILLARS_MATCH_LEADER_PRIORITY
#define BABI_PILLARS_MATCH_LEADER_PRIORITY 0
#endif

/* When Bane's room on Crossbone Isle is set up, the first pillar's sprite is
   fixed at priority 3, the lowest, instead of taking its priority from the map
   cell under it. It then draws behind Isaac and behind map layers of priority 1
   and 2. */
#ifndef BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY
#define BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY 0
#endif

/* In Bane's room on Crossbone Isle, the cell west of the first pillar, (37,
   42), and its spare copy at (62, 37) carry their own cell behaviour 61 instead
   of the first pillar's 51. An added step trigger sends 61 to the same
   first-pillar push. Play is the same either way; only the trigger table and
   map data differ. */
#ifndef BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR
#define BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR 0
#endif

/* syGetLanguage reads the language from the last letter of the cartridge
   header's game code (D, F, S, I, or English for anything else). The battle
   status and target screens choose by it at run time: the level label (Niv, Nv.
   or Lv), the HP label (KP for German, PV for French and Spanish, the edition's
   own for Italian) and the widths of the Set Djinni and cured-conditions
   windows. The ROM carries the other languages' labels for this. */
#ifndef BATTLE_LABELS_BY_HEADER_LANGUAGE
#define BATTLE_LABELS_BY_HEADER_LANGUAGE 0
#endif

/* Before the in-battle Status screen (mnBattleStatScrRun) closes its windows,
   it calls mnWindowMarkVisibleTextTiles. That marks the text tiles the tilemap
   still shows and frees every other taken text tile. This frees the tiles that
   earlier redraws left behind, which closing a window would not free, because a
   window close frees only the tiles still inside its rectangle. */
#ifndef BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 0
#endif

/* On every redraw of the in-battle Status screen, the Status screen calls
   mnWindowMarkVisibleTextTiles before redrawing. That covers opening, L or R to
   another member, and a redraw of the help line alone when the cursor moves.
   The call frees the text tiles no longer shown, so moving around the screen
   does not use up the text tile pool. */
#ifndef BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_REDRAW 0
#endif

/* The board-the-ship reminder from the Kalay docks ticket checker starts its
   event only when he actually says the line, once the ticket has been handed
   over and the party has gone ashore. It ends the event after he turns south.
   With the flag off, the handler makes an unused party-leader lookup, starts
   the event before its checks even when he says nothing, and never ends it. */
#ifndef BOARD_SHIP_REMINDER_ENDS_EVENT
#define BOARD_SHIP_REMINDER_ENDS_EVENT 0
#endif

/* Once the party has gone ashore at the Kalay docks, the ticket checker gives
   his board-the-ship reminder only when spoken to from the north. With the flag
   off, a catch-all talk row runs the reminder handler from any side the earlier
   rows do not take. Once ashore that means every side. Before going ashore it
   means north and south, where the handler says nothing. */
#ifndef BOARD_SHIP_REMINDER_ONLY_FROM_NORTH
#define BOARD_SHIP_REMINDER_ONLY_FROM_NORTH 0
#endif

/* Where the es and it battle-target code sizes windows by the cartridge's
   language at run time, mnBattleTargetRunSelection's stack frame comes out
   differently. The eighteen dead effect.ang = 0 stores, a FAKEMATCH that
   restores the other editions' spill slots, are left out. Nothing visible
   changes. */
#ifndef CARTRIDGE_LANGUAGE_AT_RUN_TIME
#define CARTRIDGE_LANGUAGE_AT_RUN_TIME 0
#endif

/* At the Lunpa Fortress Gates, when Cloak fades with Isaac still in the yard
   between the palisades, the guards' catch runs through the trigger system
   after the fade. It is raised as RAISED_PARTY_SPOTTED, the trigger that also
   catches an uncloaked Isaac. Japanese plays the catch straight from inside the
   fade's callback. */
#ifndef CLOAK_FADE_RAISES_SPOTTED_TRIGGER
#define CLOAK_FADE_RAISES_SPOTTED_TRIGGER 0
#endif

/* The staff roll is the localized release's revision, in English as every
   edition's roll is. The Japanese Public Relations section and the Special
   Thanks to the Super Mario Club testers give way to the North American
   localization staff and testers, and headings and wordings are revised. In The
   Broken Seal: GOLDEN SUN STAFF CREDITS, Sound Director, Graphic Supports,
   Artwork, and Coordination moved up with a fourth name. In The Lost Age:
   Graphic Art, P.E. Jareth Hein, and the Artwork Design section dropped. In
   both, a comma after Scenario in the copyright lines. */
#ifndef CREDITS_REVISED_FOR_LOCALIZATION
#define CREDITS_REVISED_FOR_LOCALIZATION 0
#endif

/* On Crossbone Isle's floors, moving or pushing the stone pillar when it
   already sits in the gap (its floor's passage flag is set) returns at once.
   Without the flag the pillar drops again: the fall, the dust, the whoosh, the
   40-tick wait and the cell copy all repeat. */
#ifndef CROSSBONE_ISLE_PILLAR_DROPS_ONCE
#define CROSSBONE_ISLE_PILLAR_DROPS_ONCE 0
#endif

/* Every statue in Crossbone Isle's statue room shoves Isaac back 24 pixels and
   follows him 16. The first statue follows 16 instead of 24, and the other four
   shove 24 instead of 16. Without the flag, each statue follows as far as it
   shoves: the first 24 and 24, the others 16 and 16. */
#ifndef CROSSBONE_ISLE_STATUES_SHOVE_ALIKE
#define CROSSBONE_ISLE_STATUES_SHOVE_ALIKE 0
#endif

/* The Increase item and level debug room gains two things: - a man in a white
   turban who opens an ability browser: `Psy No:` over an ability's number, name
   and description, with `B:Return`; - two standalone emote bubbles that, when
   talked to, wince or show an ellipsis and say MsgDebugWontStopYou. The room's
   level editor cat also faces east instead of southeast. */
#ifndef DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES
#define DEBUG_ABILITY_BROWSER_AND_EMOTE_BUBBLES 0
#endif

/* The Increase item and level debug room's ability browser is put out of reach.
   Its talk trigger is dropped, and the man in the white turban stands at x 80,
   off camera, instead of 848. The browser function stays compiled but is never
   called. */
#ifndef DEBUG_ABILITY_BROWSER_HIDDEN
#define DEBUG_ABILITY_BROWSER_HIDDEN 0
#endif

/* The Menu test debug room's Djinn gift (talking to the first Dora) no longer
   runs the Japanese leftover loop. That loop fills an unused 256-byte stack
   buffer with letters 16 times and steps MenuTestFillLetter. The buffer itself
   stays, so the stack frame is unchanged. Nothing visible changes. */
#ifndef DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED
#define DEBUG_DJINN_GIFT_FILL_LOOP_DROPPED 0
#endif

/* In the Increase item and level debug room, casting Force at Menardi (the
   portrait browser) opens a window with three rows of the font's Latin letters
   and marks. It replaces the Japanese motion test, in which Menardi walks on
   the spot and the cat shakes its head and is moved to 312x416. */
#ifndef DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS
#define DEBUG_FORCE_SHOWS_GLYPH_CAPTIONS 0
#endif

/* The Increase item and level debug room's level editor cat is drawn at scale
   1.6 instead of 1.7. */
#ifndef DEBUG_LEVEL_EDITOR_SMALLER
#define DEBUG_LEVEL_EDITOR_SMALLER 0
#endif

/* Entering the Menu test debug room raises Isaac to level 50 instead of 30.
   Garet, Mia and Ivan still go to 30. */
#ifndef DEBUG_MENU_TEST_ISAAC_LEVEL_50
#define DEBUG_MENU_TEST_ISAAC_LEVEL_50 0
#endif

/* Entering the Menu test debug room no longer sets the party's coins to
   777,777. */
#ifndef DEBUG_MENU_TEST_NO_START_COINS
#define DEBUG_MENU_TEST_NO_START_COINS 0
#endif

/* In the Menu test debug room, the second Dora's talk shows
   MsgTolbiTolbiShipThenTheyAreDangerous instead of running the Nut gift. The
   Nut gift stays compiled but unused. */
#ifndef DEBUG_SECOND_DORA_DANGER_MESSAGE
#define DEBUG_SECOND_DORA_DANGER_MESSAGE 0
#endif

/* In the Menu test debug room, the second Dora's sanctum test is replaced by a
   Nut gift through the ordinary party gift and full-bag dialogue:
   dbMenuTestOpenSanctum is no longer built, and dbMenuTestGiveItemToParty is.
   Her talk runs the Nut gift unless DEBUG_SECOND_DORA_DANGER_MESSAGE is also
   on. That flag is on in es and it, so only fr actually gives the Nut. */
#ifndef DEBUG_SECOND_DORA_NUT_GIFT
#define DEBUG_SECOND_DORA_NUT_GIFT 0
#endif

/* In the Menu test debug room, talking to the second Felix while facing south
   runs a message test. It sets two message ids and shows only the second,
   MsgValeComeOnHurry, in a window with no speaker. */
#ifndef DEBUG_SECOND_FELIX_MESSAGE_TEST
#define DEBUG_SECOND_FELIX_MESSAGE_TEST 0
#endif

/* In the Menu test debug room, the two Felixes browse shopkeepers' lines
   instead of opening the party member chooser and the item browser. Each shows
   one shop's lines for each way the leader faces him: - first Felix: the
   weapon, armor and item shops, and the sanctum; - second Felix: the warrior
   weapon, armor and item shops. */
#ifndef DEBUG_SHOP_MESSAGE_BROWSER
#define DEBUG_SHOP_MESSAGE_BROWSER 0
#endif

/* The debug shop, opened by the Menu test room's armor merchant, stocks Vale's
   weapon shop row instead of Vale's armor shop row. Because the shop's type
   comes from that row, it also runs as a weapon shop. */
#ifndef DEBUG_SHOP_SELLS_WEAPONS
#define DEBUG_SHOP_SELLS_WEAPONS 0
#endif

/* Vale Plaza's debug story-flag entrance (VALE_PLAZA_DEBUG_STORY_FLAGS) marks
   the story done through the storm night, Sol Sanctum, leaving Vale and
   reaching Kalay. Before, it set only Tret defeated and Kolima's curse lifted.
   Its exit, VALE_PLAZA_TO_BILIBIN, now leads to Sol Sanctum's statue instead of
   Bilibin. */
#ifndef DEBUG_STORY_PRESET_THROUGH_KALAY
#define DEBUG_STORY_PRESET_THROUGH_KALAY 0
#endif

/* In the unused battle debug value editor, holding Select returns the later
   editions' preset: 399 instead of 480 in The Broken Seal, and 544 instead of
   411 in The Lost Age. The Lost Age's other preset, held R, is 399 in every
   edition and does not change. */
#ifndef DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET 0
#endif

/* On the two-sided page of a Djinn give preview, the receiver's stats are
   worked out in trade mode, not as a plain add. Before adding the given Djinni,
   it puts on standby whatever Djinni pair slot 1 last held, if the receiver has
   that one set. That slot is left over from an earlier trade or chooser, since
   a give chooses nothing on the receiver's side. The giver's side and the
   receiver's own page are unchanged. */
#ifndef DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE
#define DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE 0
#endif

/* On entering Crossbone Isle's dragon statue room, the two logs' motion flags
   are cleared, so they no longer follow the ground height under them or fall.
   With no push event for them in that room, they stay where they are placed. */
#ifndef DRAGON_STATUE_LOGS_STAY_PUT
#define DRAGON_STATUE_LOGS_STAY_PUT 0
#endif

/* The party member chooser resets the height of all eight party figure slots
   (memberY[8]) instead of the first four. The Broken Seal's party has at most
   four members, so nothing visible changes and only the compiled code
   differs. */
#ifndef EIGHT_PARTY_FIGURES_RESET
#define EIGHT_PARTY_FIGURES_RESET 0
#endif

/* The Status screen holds the experience still needed for the next level in a
   64-bit variable instead of a 32-bit one. The subtraction and the message
   argument are 32-bit either way, so the number shown is the same and only the
   compiled code differs. */
#ifndef EXPERIENCE_NEEDED_64_BIT
#define EXPERIENCE_NEEDED_64_BIT 0
#endif

/* While the field menu's screens are up, Reveal shows no sparkles. Once the
   first command ring returns, the menu is marked open. If Reveal is active, the
   sparkles already on screen are released (the markers stay, and Reveal itself
   stays active) and the game waits a tick. No new sparkles are spawned while
   the mark is set, and it is cleared when the field menu closes. */
#ifndef FIELD_MENU_STOPS_REVEAL_SPARKLES
#define FIELD_MENU_STOPS_REVEAL_SPARKLES 0
#endif

/* The save screen's file list marks the selected file with a bar across its row
   that glows on a sine wave (colours 8 to 11 of the highlight palette), instead
   of a pointing hand to the left of it. It covers the glow task and its start
   and stop, the row fill, the redraw when the selection moves, and the hand
   cursor's variables and sprite, which the glow build does not have. */
#ifndef FILE_LIST_GLOW_BAR_CURSOR
#define FILE_LIST_GLOW_BAR_CURSOR 0
#endif

/* Every bag is full and the player picks a carried item to drop for a gift.
   English no longer refuses a member who already carries a full stack
   (INVENTORY_STACK_MAX) of the gift and asks again. It drops the picked stack
   anyway, so the gift may then not fit: the picked item is lost and the 'gave'
   line shows actor -1, or a stack of one is left. */
#ifndef GIFT_DROP_SKIPS_FULL_STACK_CHECK
#define GIFT_DROP_SKIPS_FULL_STACK_CHECK 0
#endif

/* A gift fits in the bags and goes to a member other than the leader. The 'gave
   <item> to <member>' message that follows now waits for a button press.
   Before, it also closed by itself once the treasure jingle had finished: the
   mode loses FIELD_MESSAGE_WAITING_FOR_SOUND and keeps only
   FIELD_MESSAGE_ZOOM. */
#ifndef GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON
#define GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON 0
#endif

/* When giving an item to a party member whose bag is full and who holds none of
   it, the Give screen draws only "Trade for what?". Without the flag it first
   draws the "None in stock" line and then writes the prompt over it on the same
   row. */
#ifndef GIVE_TRADE_PROMPT_REPLACES_STOCK
#define GIVE_TRADE_PROMPT_REPLACES_STOCK 0
#endif

/* mnGlyphDrawArm no longer reserves an unused 64-byte local array (16 words) on
   its stack. Nothing reads it, so nothing on screen changes; only the compiled
   stack frame differs (sub sp, #100 in ja, #44 in the other editions). */
#ifndef GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED 0
#endif

/* At Goma Cave Entrance, the queued waterfall sound (SE_AMBIENT_WATER) plays
   again at two points in the event for Move on the ivy-bound stump: at its
   start, and right after Ivan's Whirlwind blows the ivy off. Without this
   feature the sound comes back only when the Psynergy cast ends, after the
   whole event. */
#ifndef GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT
#define GOMA_STUMP_WATERFALL_RESUMES_IN_EVENT 0
#endif

/* Pulling the cartridge out halts the game. The GamePak interrupt is enabled
   and its handler is syHalt, a halt loop that runs from IWRAM. In TBS that loop
   sits in the interrupt table's Dma2 and Dma3 slots, in place of two ignore
   handlers. With the flag off, the GamePak interrupt stays disabled, no handler
   is installed and the table keeps syIrqIgnore, syIrqIgnore. */
#ifndef HALT_ON_CARTRIDGE_REMOVED
#define HALT_ON_CARTRIDGE_REMOVED 0
#endif

/* Imil's moved snowman is marked moved (FLAG_IMIL_SNOWMAN_MOVED), given sprite
   priority 3 and made a block at cell (15, 7) only when it actually stands on
   (15, 7). The other editions do all three wherever Move left it. The check
   runs after Move and again when the village reloads with the snowman already
   moved. */
#ifndef IMIL_SNOWMAN_PLACED_ONLY_ON_ITS_CELL
#define IMIL_SNOWMAN_PLACED_ONLY_ON_ITS_CELL 0
#endif

/* In English, Venus Lighthouse's two inner reliefs, with the true-heart words,
   have no examine trigger and say nothing, although their text is in the
   English catalogue. The relief reader then always shows the first relief's
   words, since that relief is the only one left that can call it. The other
   editions keep the trigger and choose each relief's message by the examined
   cell. */
#ifndef INNER_RELIEFS_SILENT
#define INNER_RELIEFS_SILENT 0
#endif

/* In Lalivero, Iodem comes out of his door and is placed in the doorway at (72,
   312) in every edition. With this feature, his leftover move target (tgtZ) is
   then cleared and he is placed in the doorway a second time. He then walks to
   the fixed point (72, 328), instead of 16 pixels south of where he stands.
   Both walks end at the same point, so the visible effect is only a guard
   against a stale target left from an earlier move. */
#ifndef IODEM_DOOR_WALK_RESET
#define IODEM_DOOR_WALK_RESET 0
#endif

/* When Use is chosen on the Item screen for an item that sets off a scene
   event, the screen reports the member whose items the list is showing.
   Japanese reports the member first picked at 'Whose item?'. So after switching
   holders with L or R, Japanese counts the item in the wrong member's
   inventory, says 'But nothing happens.' if they hold none, and names them as
   the user. The command loop's locals are rewritten with it: the Give target
   gets its own local, and the member id is kept in command. */
#ifndef ITEM_EVENT_USED_BY_SHOWN_HOLDER
#define ITEM_EVENT_USED_BY_SHOWN_HOLDER 0
#endif

/* After the item menu (and the Psynergy menu, which shares it) shows a message
   in its own small fitted window, that window is closed once the message is
   dismissed, instead of being kept open and resized for the next message. */
#ifndef ITEM_MENU_CLOSES_MESSAGE_WINDOW
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW 0
#endif

/* While the Item screen is open, it suspends the field's tasks (tile
   animations, weather, palette blend), saves BG char block 1 (8192 bytes),
   fills it with colour 3 and lends it to text as the second range of text
   tiles. On closing it takes the tiles back, restores the block and resumes the
   field's tasks. */
#ifndef ITEM_MENU_EXTRA_TEXT_TILES
#define ITEM_MENU_EXTRA_TEXT_TILES 0
#endif

/* mnItemMenuGetItemQuantity, which gives the Give screen's "in stock" number
   and its full-bag test, adds up every stack of the item a member holds instead
   of returning the first stack's count. */
#ifndef ITEM_QUANTITY_ADDS_ALL_STACKS
#define ITEM_QUANTITY_ADDS_ALL_STACKS 0
#endif

/* In Vale's Sanctum, the West Healer asks whether Isaac's new friends are
   Adepts. Ivan is first brought into the room, and on a no he gives the silent
   '......' together with Mia. In Japanese, Garet and Mia give it and Ivan is
   not placed. */
#ifndef IVAN_REACTS_TO_NEW_FRIENDS_QUESTION
#define IVAN_REACTS_TO_NEW_FRIENDS_QUESTION 0
#endif

/* German gives KeysDirection a 12-byte slot and TasksInitialized a 4-byte one
   (4 and 12 elsewhere), so IdleSleepEnabled through TasksInitialized lie a
   further 8 bytes later. Nothing reads them by fixed address, so only the
   layout differs. */
#ifndef IWRAM_GAP_AFTER_KEYS_DIRECTION
#define IWRAM_GAP_AFTER_KEYS_DIRECTION 0
#endif

/* German's IWRAM variables below the heap start 16 bytes later. A 16-byte pad
   goes before OamDepthLists and the same 16 bytes come out of the number
   buffer's spare room, so the heap does not move. The fixed addresses of
   BgScrollWork and VramCacheEntries move by 16 with it. Behaviour and text are
   unchanged. */
#ifndef IWRAM_LAYOUT_SHIFT
#define IWRAM_LAYOUT_SHIFT 0
#endif

/* After the statue falls in Mercury Lighthouse's upper gate room, the party
   returns to the room of jets. With this feature, the west-east pipe and the
   switch pipe are placed where their area flags say they were pushed. The jet
   the switch pipe selects is also restarted: westward with the statue's
   sparkles, eastward, southward, or none. This is what the ordinary return from
   the gate already does in every edition. Without the feature, both pipes are
   always placed in their solved spots, (816, 608) and (776, 576), and the
   eastward jet starts. */
#ifndef JET_ROOM_RESTORED_AFTER_STATUE_FALL
#define JET_ROOM_RESTORED_AFTER_STATUE_FALL 0
#endif

/* Scripted jumps and hops land on the centre of a map cell along the nearest
   compass direction, instead of at a fixed pixel offset from Isaac's own
   position (which in Japanese can be diagonal or off the grid). This covers
   Crossbone Isle and its stepping stones, Goma Cave's pillar gap, Vault's inn
   attic, Mogall Forest's river log, Kolima's log, Venus Lighthouse's gaps,
   Tunnel Ruins and Vale Cave's riding log. At Goma Cave and the attic, the
   scene's own jump is replaced by the engine's fdTryJumpForward, which also
   brings its check of the cell in between and its landing behaviour. */
#ifndef JUMP_LANDS_ON_CELL_CENTER
#define JUMP_LANDS_ON_CELL_CENTER 0
#endif

/* On a return to the Kalay Tunnel treasure room with the party's position kept
   (after a battle, for example), the ground mask follows the leader only if it
   was following him before. The Japanese edition sets the follow flag first, so
   there the mask always goes back to following him. On an ordinary entry both
   versions behave the same. */
#ifndef KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING
#define KALAY_TUNNEL_MASK_REMEMBERS_FOLLOWING 0
#endif

/* The Nut in Kolima Forest's glade is placed by its own caught flag,
   FLAG_KOLIMA_FOREST_NUT_CAUGHT (the flag catching it sets), instead of Vale's
   FLAG_VALE_NUT_CAUGHT. So catching Vale's Nut no longer stops this one being
   placed. */
#ifndef KOLIMA_NUT_CHECKS_OWN_FLAG
#define KOLIMA_NUT_CHECKS_OWN_FLAG 0
#endif

/* After Kalay is reached, the jar by Kraden's cottage in Vale still gives its
   Elixir if it was not taken before. It uses the same one-time flag as the
   daytime row, so it is never given twice. The other editions have no jar row
   in the after-Kalay table, so an Elixir not taken by then is lost. */
#ifndef KRADENS_JAR_ELIXIR_AFTER_KALAY
#define KRADENS_JAR_ELIXIR_AFTER_KALAY 0
#endif

/* The entered-password buffer reserves 320 symbols instead of a Gold password's
   260. Nothing reads or writes past 260, so only the memory layout changes. The
   Broken Seal's file screen gets 60 more bytes of stack. In The Lost Age, the
   60 spare bytes that the Japanese edition keeps after the decoded password
   move into the buffer, and the total size of the region stays the same. */
#ifndef LARGER_PASSWORD_BUFFER
#define LARGER_PASSWORD_BUFFER 0
#endif

/* In the stacked pillars room off Crossbone Isle's seventh floor, Isaac's
   height is set to the ground height under him before he jumps across or pushes
   a pillar. */
#ifndef LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR
#define LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR 0
#endif

/* After a losing throw in Lucky Dice, the game reopens the Throw / Quit help
   window, as it already does at the start and after a payout. Japanese goes
   back to aiming with no help window. Its next throw, B or running out of coins
   then closes a window that is already closed. */
#ifndef LUCKY_DICE_HELP_AFTER_LOSS
#define LUCKY_DICE_HELP_AFTER_LOSS 0
#endif

/* Lucky Wheels remembers which prompt window is open, and opens and draws it
   only when the prompt changes, instead of closing and reopening it every
   tick. Because the state is remembered, two more things work:
   - The all-held prompt gains a second line ('cannot pull the lever'), drawn on
	 the next tick.
   - After a losing pull, the 'pull the lever' line is drawn once. */
#ifndef LUCKY_WHEELS_PROMPT_DRAWN_ONCE
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE 0
#endif

/* Lucky Wheels draws the count of Game Tickets left after the bet with three
   digits from x 56, instead of two digits from x 64, so the count ends where it
   did before. */
#ifndef LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT
#define LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT 0
#endif

/* When the Lunpa Fortress Gates load with Cloak still on, Cloak's gateway
   rewrite is applied again. This happens on a reload that keeps the party in
   place, such as continuing a save made there. The three cells behind the
   guards, (7,11) to (9,11), become Cloak cells, as casting Cloak there makes
   them. The other editions leave them as guarded ground, so a cloaked leader
   who steps on them loses Cloak. */
#ifndef LUNPA_GATES_RELOAD_REAPPLIES_CLOAK
#define LUNPA_GATES_RELOAD_REAPPLIES_CLOAK 0
#endif

/* In Lunpa, Reveal copies its cell attributes over the hidden puddle's 3x3
   cells. If the puddle is frozen at that moment, these editions then put back
   the ice pillar's top and side cells. That can only happen when Reveal is cast
   again while an earlier Reveal still shows the frozen pillar. */
#ifndef LUNPA_REVEAL_KEEPS_ICE_PILLAR_COLLISION
#define LUNPA_REVEAL_KEEPS_ICE_PILLAR_COLLISION 0
#endif

/* Coming back from the Manticore battle in Lamakan Desert also sets
   FLAG_KALAY_REACHED. In normal play that flag is already set by then, because
   every Kalay scene sets it on entry, so a player sees no difference. */
#ifndef MANTICORE_MARKS_KALAY_REACHED
#define MANTICORE_MARKS_KALAY_REACHED 0
#endif

/* When the pad is released, The Broken Seal's map screen cursor slides onto the
   nearest area marker. It starts at speed 2 and keeps accelerating, then lands
   once it is within 1. A marker shows its name, and catches the cursor, within
   a squared distance of 64 instead of 100. The marker loop's setup and end test
   are also reordered the way the rewrite was compiled. */
#ifndef MAP_SCREEN_CURSOR_SNAPS
#define MAP_SCREEN_CURSOR_SNAPS 0
#endif

/* The McCoy palace grounds get one more actor, a patch of ivy (SPRITE_IVY) at
   cell (8, 24), just past the grounds' south edge. The source says it covers a
   dark opening at the foot of the cliff. It is only visual: no exit or trigger
   answers it. The earlier editions leave the spot bare. */
#ifndef MCCOY_PALACE_GROUNDS_IVY
#define MCCOY_PALACE_GROUNDS_IVY 0
#endif

/* Mercury Lighthouse's middle floor has a gate room with a terrace. When the
   party enters that room from the lower floor or from the pipes, a tick task
   starts. While the leader is on the ground layer in rows 13 to 18 (the
   terrace), the task stops the ground mask from drawing on him. Everywhere else
   the mask follows him as before. The ground mask is what draws raised ground
   in front of an actor over him. */
#ifndef MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED
#define MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED 0
#endif

/* While Mind Read is active, the all-pages text measure (mnTextMeasureAllPages,
   which sizes a window to hold every page of a message) no longer adds 2 pixels
   to the first page's widest line. That width also sets the spread between
   words. */
#ifndef MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER 0
#endif

/* While Mind Read is active, the first-page text measure (mnTextMeasureDecoded,
   which sizes a window to a message's first page) no longer adds 2 pixels to
   the measured width. That width also sets how far the spare width is spread
   between words. */
#ifndef MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER 0
#endif

/* While Mind Read is active, each typed character is no longer drawn 8 pixels
   further right. */
#ifndef MIND_READ_TEXT_NOT_INDENTED
#define MIND_READ_TEXT_NOT_INDENTED 0
#endif

/* While Mind Read is active, a window sized from its text is no longer made 2
   tiles wider and kept off the screen's last column. mnWindowFitOnScr fits it
   like any other window. */
#ifndef MIND_READ_WINDOW_NOT_WIDENED
#define MIND_READ_WINDOW_NOT_WIDENED 0
#endif

/* Re-entering Mogall Forest's second area by one of its three river entrances,
   with the river log already rolled into the stream, sets cells (35, 29) to
   (35, 32) on map layer 0 to open ground. Only (35, 29) held anything: a
   log-push trigger beside where the rolled log now lies. It is also the
   westmost of the cells from which the south log is pushed north, so that push
   cell goes too. It was never the rolled log's old collision. */
#ifndef MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN
#define MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN 0
#endif

/* The hiding place of the east ape in the third area of Mogall Forest
   (ACTOR_THIRD_AREA_EAST_APE_STUMP, a blank actor at (744, 400) that shares the
   ape's sprite) is placed only while the ape has not come out, the way the
   other apes' hiding places are. The Japanese edition places it always. */
#ifndef MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT
#define MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT 0
#endif

/* Entering the river clearing of Mogall Forest's third area (from the stumps,
   from the side paths east or south, or from the bridges) after its east ape
   has come out moves that ape to (0, 0) again, out of sight. Every edition
   already moves it there at the end of the event that brings it out. Without
   this, entering the river clearing only hides the ape while it has not come
   out. Entering the stumps clearing, the chest, the bridges clearing or
   arriving from the World Map does not move it. */
#ifndef MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY
#define MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY 0
#endif

/* The east ape in the third area of Mogall Forest (744, 400) is placed only
   while it has not come out (FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT clear),
   the way the other apes are. The Japanese edition and the later European
   editions place it always. */
#ifndef MOGALL_THIRD_AREA_EAST_APE_PLACED_UNTIL_OUT
#define MOGALL_THIRD_AREA_EAST_APE_PLACED_UNTIL_OUT 0
#endif

/* In Mercury Lighthouse, the north jet in the Nut chest's room carries Isaac to
   (696, 592) and the event waits until he arrives. The jet then carries him 8
   pixels further, to (696, 600), and the event pauses 30 ticks. Without this
   feature the event only sets his destination to (696, 592) and waits for that
   move. */
#ifndef NUT_ROOM_JET_CARRIES_FURTHER
#define NUT_ROOM_JET_CARRIES_FURTHER 0
#endif

/* Two narrow glyphs share one 16-pixel sprite cell only when their widths add
   up to 14 or less in outlined text (15 in plain text), because outlining
   widens each glyph by a pixel. Without this, an outlined pair could come to 17
   pixels and spill into the next row of the cell. */
#ifndef OUTLINED_GLYPH_PAIRS_FIT_CELL
#define OUTLINED_GLYPH_PAIRS_FIT_CELL 0
#endif

/* At a page break, no next-page arrow is drawn, and the arrow is not marked as
   shown, when the message window has zero width or height. */
#ifndef PAGE_ARROW_SKIPS_EMPTY_WINDOW
#define PAGE_ARROW_SKIPS_EMPTY_WINDOW 0
#endif

/* At a page break, typing resets the text colour, outline and line spacing to
   their defaults. Both window measures (the first-page measure and the
   all-pages measure) reset the outline at the same point. The measures track no
   other style, since outline is the only one that changes a glyph's width. So
   the width measured for the next page matches the plain style it is typed
   in. */
#ifndef PAGE_BREAK_RESETS_STYLE
#define PAGE_BREAK_RESETS_STYLE 0
#endif

/* In debug mode, the Pause menu's heading sits 8 pixels lower, at y -16 instead
   of -24 above its window. The window starts higher then, because of the extra
   debug rows. Outside debug mode it stays at -24. */
#ifndef PAUSE_HEADING_LOWER_IN_DEBUG
#define PAUSE_HEADING_LOWER_IN_DEBUG 0
#endif

/* While the Psynergy menu is open, it suspends the field's tasks, saves BG char
   block 1 (8192 bytes), fills it with colour 3 and lends it to text as the
   second range of text tiles. On closing it takes the tiles back, restores the
   block and resumes the field's tasks. This is the Item screen's change, but it
   starts in German, so English lacks it. */
#ifndef PSYNERGY_MENU_EXTRA_TEXT_TILES
#define PSYNERGY_MENU_EXTRA_TEXT_TILES 0
#endif

/* Venus Lighthouse's first relief wall is read through one examine trigger. All
   five of its cells carry the first relief's behaviour, so the four extra
   triggers are dropped. In Japanese each cell has its own behaviour and its own
   trigger, and every one shows a copy of the same words. */
#ifndef RELIEF_WALL_SINGLE_TRIGGER
#define RELIEF_WALL_SINGLE_TRIGGER 0
#endif

/* The help line for the Resistance Down condition on the battle Status screen
   shows the amount Resistance dropped by without a minus sign ("dropped by 20",
   not "dropped by -20"), as Attack, Defense and Agility Down already do in
   every edition. Resistance Up is unaffected, since its amount is always
   positive. */
#ifndef RESISTANCE_DOWN_HELP_NO_MINUS
#define RESISTANCE_DOWN_HELP_NO_MINUS 0
#endif

/* If Reveal is cast again while it is still running, revealActive stays 1
   through the one-tick wait and the scene's Reveal event. The other editions
   set it to 0 first. A fresh cast behaves the same in every edition. */
#ifndef REVEAL_RECAST_STAYS_ACTIVE
#define REVEAL_RECAST_STAYS_ACTIVE 0
#endif

/* Some rooms open with a different screen transition than in Japanese. Champa's
   cliff caves use the window shape instead of an interlaced wipe. Champa's and
   Lemuria's rooms open around the leader instead of in the shape. The Gabomba
   Catacombs, Gaia Rock and Kibombo's rooms use an interlaced wipe instead of
   the shape. Alhafra's jail opens around the leader when entered from Alhafra
   or from under the crate. Taopo Swamp's mud room and entrance fade in and its
   other rooms use an interlaced wipe, where Japanese uses the shape for every
   room. In The Broken Seal, Lunpa Fortress's guard room uses random bars after
   its battle, as its other entrances do, instead of opening around the
   leader. */
#ifndef REVISED_ENTRANCE_TRANSITIONS
#define REVISED_ENTRANCE_TRANSITIONS 0
#endif

/* After a cable send, the game waits one more tick after closing the 'Sending
   data' window before it goes back to the file menu. The explicit clear before
   the close is dropped as well, which changes nothing on screen, because
   closing a window always clears it first. */
#ifndef SENDING_WINDOW_WAITS_A_TICK_AFTER_CLOSING
#define SENDING_WINDOW_WAITS_A_TICK_AFTER_CLOSING 0
#endif

/* When a battle target is chosen, the Set Djinni window is kept on screen by
   its real width: 9 tiles outside Japanese, rather than the Japanese 8. Against
   the right edge of the screen, its last column therefore no longer runs past
   the screen. BUGFIX also turns it on in every edition of both games. */
#ifndef SET_DJINNI_WINDOW_FITS_SCREEN
#define SET_DJINNI_WINDOW_FITS_SCREEN 0
#endif

/* When 1000 tries at making room for the Shaman's Rod in Ivan's bag run out,
   his first item that is not indispensable is discarded, and his last slot is
   blanked only if every item is indispensable. The other editions always blank
   the last slot, key items included. */
#ifndef SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS
#define SHAMANS_ROD_SPARES_INDISPENSABLE_ITEMS 0
#endif

/* In the game's ending, the Lemurian ship starts sailing west 30 ticks after
   the curtains begin to close, instead of 80. */
#ifndef SHORTER_ENDING_SHIP_DEPARTURE_DELAY
#define SHORTER_ENDING_SHIP_DEPARTURE_DELAY 0
#endif

/* In the Moving Sidewalk attendant's demonstration at the Colosso, the flinch
   effect attached over Isaac is never removed. It stays on him through his
   "..." and angry emotes and the rest of the scene. Without the flag it is
   removed just before the "..." emote. */
#ifndef SIDEWALK_DEMO_KEEPS_LEADER_EFFECT
#define SIDEWALK_DEMO_KEEPS_LEADER_EFFECT 0
#endif

/* On Venus Lighthouse's fourth floor, the socket-pillar routine first clears
   the field's busy flag and, if Reveal is active, ends it. The routine runs
   when Move is used on a socket pillar, when Isaac walks into one to push it or
   jump a gap, and when the pillar room is set up on arrival. */
#ifndef SOCKET_PILLAR_ENDS_REVEAL
#define SOCKET_PILLAR_ENDS_REVEAL 0
#endif

/* In Sol Sanctum's statue-puzzle room, the front and fastest row of scrolling
   sky clouds is drawn 6 pixels below the other two rows instead of 8, so it
   sits 2 pixels higher. */
#ifndef SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED
#define SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED 0
#endif

/* Link-battle opponents' names get their 'Enemy' label (MsgEnemyLabel) measured
   up to 8 letters instead of 5, with a space between the label and the name
   instead of none. This covers The Broken Seal's Battle Arena and The Lost
   Age's link lobby, plus each game's unused link-battle receiver. */
#ifndef SPACED_ENEMY_NAME_PREFIX
#define SPACED_ENEMY_NAME_PREFIX 0
#endif

/* For a speaker with a portrait, the speech window is bottom-anchored when the
   portrait sits below the message, instead of when the message sits above the
   speaker. Speakers without a portrait are unchanged. The two tests differ only
   when the screen-edge clamp moves the portrait to the other side of the
   message, or when the message top equals the speaker's row. */
#ifndef SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT
#define SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT 0
#endif

/* The battle Status screen's eight stat labels, Attack to Resist, are numbered
   forward from MsgOwnerStatusAttack instead of back from MsgOwnerStatusExp. Exp
   is Attack + 10 in every edition's message table, so the same messages show;
   only the compiled code differs (a second literal-pool word and adds instead
   of subtracts). */
#ifndef STATUS_LABELS_FROM_ATTACK
#define STATUS_LABELS_FROM_ATTACK 0
#endif

/* Before the Status menu's Psynergy page or equipment page draws a description,
   it clears only the window's first 8-pixel text line
   (mnWindowClearInteriorTiles 0,0,224,8), not the whole window. */
#ifndef STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY
#define STATUS_MENU_CLEARS_DESCRIPTION_LINE_ONLY 0
#endif

/* The Status menu saves 8192 bytes of BG char block 1 instead of 2048, after
   the palette copies instead of before. It fills them with colour 3 and lends
   their first 128 tiles (tile 512 on) to text as the second text tile range. On
   closing it takes that range back before restoring the block. */
#ifndef STATUS_MENU_EXTRA_TEXT_TILES
#define STATUS_MENU_EXTRA_TEXT_TILES 0
#endif

/* In the south half of Suhalla Desert, exit 4 into the desert caves' west
   entrance fires only while the leader faces north, within an eighth of a turn.
   In Japanese it fires on any step. The east cave's exit 3 and exits 1 and 2
   stay unconditional in every edition. */
#ifndef SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH
#define SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH 0
#endif

/* At the Suhalla Gate checkpoint after Babi's audience, the three wounded
   soldiers no longer block the party with their actor collision. The map cells
   under them become walls instead:
   - They are placed with scripts that turn collision off and are moved 4 px
	 nearer the middle of their cells.
   - On entry, the two cells under each soldier are copied from the wall cells
	 at (0, 0).
   - When the vanishing soldier goes, his cells are reopened from (29, 19).
   - The approach cells (7, 26) and (8, 26) are reset from (11, 26) instead of
	 from (9, 26), which are now the east guard's wall cells. */
#ifndef SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS
#define SUHALLA_GATE_WOUNDED_SOLDIERS_BLOCK_BY_WALL_CELLS 0
#endif

/* Retreat out of the Suhalla Gate checkpoint names
   SCENE_SUHALLA_GATE_CHECKPOINT as a constant instead of the current scene.
   Both are inside if(sceneId == SCENE_SUHALLA_GATE_CHECKPOINT), so Retreat
   leads to the same place in every edition; only the compiled code differs. It
   is part of the same scInit rework of the French, Spanish and Italian editions
   as the wounded soldiers laid out without collision. */
#ifndef SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION
#define SUHALLA_GATE_WOUNDED_SOLDIERS_NO_COLLISION 0
#endif

/* In the upper-floor three-guards scene in Lunpa Fortress, the west guard turns
   to face the party leader after walking into place, before he nods and speaks.
   In the Japanese edition he nods and speaks still facing the way he walked. */
#ifndef THREE_GUARDS_WEST_FACES_LEADER
#define THREE_GUARDS_WEST_FACES_LEADER 0
#endif

/* The unnamed span at the start of the title's globals, before
   TitleRevealTick, is 54 bytes instead of 58. TitleRevealTick and
   TitleSprites therefore sit 4 bytes earlier. No code names or reads the span,
   so only the layout of memory changes. */
#ifndef TITLE_GLOBALS_LEAD_SHORTER
#define TITLE_GLOBALS_LEAD_SHORTER 0
#endif

/* During Tolbi's festival (before Babi is saved and before Colosso ends), the
   jar with a Lucky Medal behind the town's easternmost house can be searched
   only while the leader faces north. On finals day and after Colosso it can
   still be searched facing any way, in every edition. */
#ifndef TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH
#define TOLBI_FESTIVAL_JAR_NEEDS_FACING_NORTH 0
#endif

/* The camera can go 8 pixels further in Tolbi's inn: its basement passage and
   all four guest rooms. The bounds become 744, 952 and 936 instead of 736, 944
   and 928, so the last 8 pixels of these rooms are no longer cut off: the
   passage's south openings, the southern rooms' east walls and the northern
   rooms' doorways. The upstairs inn room and Kalay's inn (stucco_rooms_a.c) are
   unchanged. */
#ifndef TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS
#define TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS 0
#endif

/* On Tolbi's ship deck, stepping onto the small middle stairway (cell 13, 41)
   while facing west walks Isaac to its middle and takes him below at once,
   instead of needing him to keep walking west into it. It covers the new event
   and the stairway row in all four of the deck's trigger tables. */
#ifndef TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP
#define TOLBI_SHIP_STAIRWAY_TAKEN_ON_STEP 0
#endif

/* Backing out of the item menu's trade list no longer writes -1 over the item
   the giver chose. A following Drop therefore names the right item in its
   confirmation. The drop itself always removed the right item. */
#ifndef TRADE_CANCEL_KEEPS_CHOSEN_ITEM
#define TRADE_CANCEL_KEEPS_CHOSEN_ITEM 0
#endif

/* The transfer method menu's help (Password or Cable/Link) releases its
   window's text sprites with mnWindowSprClearList without first redrawing the
   window empty, as mnWindowSprPrepareForRedraw does in the other editions. It
   draws Password's help when the selection moves onto Password and the other
   method's only when it moves off Password, setting the shown selection after
   drawing. With the menu's two icons that is every move, and the text is window
   sprites, so the help looks the same. */
#ifndef TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW
#define TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW 0
#endif

/* The Password/Cable transfer method menu is never centred and gets no label
   window: its two icons stay at their default x of 32 and 56 on the bottom row.
   Only the password level menu (Gold/Silver/Bronze) is centred. */
#ifndef TRANSFER_METHOD_MENU_UNCENTRED
#define TRANSFER_METHOD_MENU_UNCENTRED 0
#endif

/* During a screen transition, a window mask keeps the windows in view while the
   transition covers the rest. With this feature the mask counts a left or right
   border tile where a divider meets the border (a side tee, tiles 26 and 27) as
   the window's edge, so that row of the window stays in view. The top and
   bottom tees are not counted in any edition. */
#ifndef TRANSITION_MASK_COUNTS_SIDE_TEES
#define TRANSITION_MASK_COUNTS_SIDE_TEES 0
#endif

/* An uncalled function, mnBusyWait, which spins through an empty 256-pass loop,
   is compiled into the ROM. Nothing calls it, so only the ROM's contents
   differ. */
#ifndef UNUSED_BUSY_WAIT
#define UNUSED_BUSY_WAIT 0
#endif

/* The six-byte table of the HP and PP labels' tile numbers is left out of the
   ROM. It sits after the party status's divider column offsets, and nothing
   reads it, so only the position of the data after it changes and nothing
   visible differs. In TLA it is WindowUnusedPartyColumnBytes; in TBS it is the
   bytes after WindowPartyColumnOffsets. */
#ifndef UNUSED_HP_PP_LABEL_TILES_DROPPED
#define UNUSED_HP_PP_LABEL_TILES_DROPPED 0
#endif

/* Every time Vale is entered before Kalay is reached, scInit gives Dora a fixed
   sprite priority of 2 instead of the automatic one taken from the cell under
   her. This includes entries after the party has left. To fit the call in,
   scInit's Kalay test becomes an if/else and the roof and bridge redraw for a
   kept party position is copied into each branch. That restructuring changes
   only the compiled code. */
#ifndef VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY
#define VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY 0
#endif

/* In Vale, Dora is placed by the house at (373, 887), 2 pixels further north
   than (373, 889). This applies in the opening's placement table and in the
   roof repair's placement table. It is the spot the roof-mended scene walks her
   back to in every edition, so after the scene she stays where she was
   placed. */
#ifndef VALE_DORA_SPOT_MATCHES_ROOF_SCENE
#define VALE_DORA_SPOT_MATCHES_ROOF_SCENE 0
#endif

/* Pushing Garet's training stone below the cliff (scValePushPillar) no longer
   starts with a lookup of the stone (before or after Kalay) whose result is
   thrown away. Nothing visible changes; only the compiled code differs. */
#ifndef VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED
#define VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED 0
#endif

/* On the stormy night in Vale's plaza, the second aide (the second strongman,
   ACTOR_STORM_SECOND_AIDE) stands at (198, 779) instead of (204, 783), 6 pixels
   west and 4 pixels north. He stands there all night, both during the event and
   when talked to. */
#ifndef VALE_PLAZA_SECOND_AIDE_MOVED_NORTHWEST
#define VALE_PLAZA_SECOND_AIDE_MOVED_NORTHWEST 0
#endif

/* On the stormy night, as the first aide runs off past the second aide, the
   second aide's sprite priority is pinned at 1. The pin starts right after the
   grandfather's "We're counting on you" and lasts the 20 or so ticks of the
   first aide's run. Just before the second aide runs off himself, his priority
   goes back to automatic. */
#ifndef VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED
#define VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED 0
#endif

/* In the prologue by the river, a per-tick task keeps Garet and the exhausted
   strongman at the leader's sprite priority. It now sets that priority through
   fdActorSetSprPriority, so their shadows follow the leader's priority too.
   Before, only their sprites did, and their shadows kept whatever priority they
   had. */
#ifndef VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY
#define VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY 0
#endif

/* At the end of the roof mending (scValeRoofMended), Garet jumps after Dora and
   lands at (374, 829) instead of (374, 827), 2 pixels further south. He is then
   set exactly on that spot. His walk east along the roof that follows ("Wait!
   Not there!") ends on the same row, at (403, 829), because it uses the same
   value. */
#ifndef VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH
#define VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH 0
#endif

/* When the roof-mended event (scValeRoofMended) ends, the leader is set to
   height 160 with no vertical target and no vertical speed. Without this, he
   gets control back still carrying the upward speed and vertical target left
   over from climbing down the ladder. The Japanese release only restores his
   motion flags. */
#ifndef VALE_ROOF_MENDED_LEADER_GROUNDED
#define VALE_ROOF_MENDED_LEADER_GROUNDED 0
#endif

/* In the chase round the beds at Vault's inn, when Ivan is within 8 pixels
   (half a cell) of the leader, he takes the leader's heading to pick his next
   stop, even if the leader is not pushing toward him. In the Japanese edition
   Ivan takes the leader's heading only when the leader pushes toward him from
   within 20 pixels; otherwise he goes his own way. */
#ifndef VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE
#define VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE 0
#endif

/* If the player opens a menu while the party corners the suspects at Vault's
   inn, the leader is first stopped (fdLeaderStopMoving) and then Garet says
   they have no time for that. In the Japanese edition the leader keeps moving
   while Garet speaks. */
#ifndef VAULT_CHASE_MENU_STOPS_LEADER
#define VAULT_CHASE_MENU_STOPS_LEADER 0
#endif

/* Once the thieves are jailed, Vault's mayor is put back at his own spot by the
   east wall, (536, 451), where his placement row stands him. The other editions
   put him a step south, at z 464. x is the same in every edition. */
#ifndef VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES
#define VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES 0
#endif

/* In Vault as revisited after Kalay, when the party returns with its position
   kept (as from a battle), the flag that the bell called Sap out is kept, so
   Sap stays on the cliff top instead of going back into its tree. This makes
   the revisit match the first visit (vault.c), which keeps Sap in every
   edition. */
#ifndef VAULT_SAP_STAYS_AFTER_BATTLE
#define VAULT_SAP_STAYS_AFTER_BATTLE 0
#endif

/* When Isaac drops through the hole in Venus Lighthouse's first floor, his
   motion flags are cleared at the start of the event. While he is walked back
   to the edge and kicks, he neither follows the ground nor falls. When the
   floor opens, the flags are set back to ACTOR_FOLLOWS_TERRAIN |
   ACTOR_FALLS. */
#ifndef VENUS_HOLE_DROP_HOLDS_LEADER
#define VENUS_HOLE_DROP_HOLDS_LEADER 0
#endif

/* When Isaac drops through the hole in Venus Lighthouse's first floor, the edge
   row (row 34) takes the hole row's (row 35) cell behaviours across columns 33
   to 39, instead of 34 to 38: one more column on each side. */
#ifndef VENUS_HOLE_DROP_OPENS_WIDER
#define VENUS_HOLE_DROP_OPENS_WIDER 0
#endif

/* On Venus Lighthouse's ground floor there are no random battles while Reveal
   shows the hidden doorway and stairs. Casting Reveal sets
   FLAG_NO_ENCOUNTERS_IN_AREA, and the flag is cleared when Reveal fades. */
#ifndef VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS
#define VENUS_LIGHTHOUSE_REVEAL_STOPS_ENCOUNTERS 0
#endif

/* mnTextGetWideStringWidth is used only to centre a battle target's name under
   the cursor. With this feature it counts 10 pixels for every font code, the
   kana marks included, except the space, which still counts 4. A code above the
   font counts nothing. Without it, each glyph counts its own width, the kana
   marks count nothing and a kanji counts 10. */
#ifndef WIDE_STRING_FIXED_WIDTH
#define WIDE_STRING_FIXED_WIDTH 0
#endif

/* The window-closed test no longer counts a NULL window as closed. It reads the
   window's flags and duration through the pointer without checking for NULL
   first. */
#ifndef WINDOW_IS_CLOSED_NO_NULL_CHECK
#define WINDOW_IS_CLOSED_NO_NULL_CHECK 0
#endif

/* The window system's two init functions (field and battle) clear the window
   state and blank its tilemap with DMA fills from a volatile word, instead of
   calling syWordsClear and syWordsFill. Memory ends up the same; only the
   compiled code differs. */
#ifndef WINDOW_STATE_DMA_FILL
#define WINDOW_STATE_DMA_FILL 0
#endif

/* World Map steps are probed 7 pixels ahead, straight and 22.5 and 45 degrees
   either side, and all five probes must be clear. A blocked step tries eight
   headings up to 90 degrees off, each with the same five probes. Japanese
   probes 8 pixels ahead, straight and 30 degrees either side, and tries six
   headings up to 67.5 degrees. */
#ifndef WORLD_MAP_FIVE_PROBE_STEPS
#define WORLD_MAP_FIVE_PROBE_STEPS 0
#endif

/* On the World Map, the R/L shortcut opens the screen of the key just pressed,
   so pressing L while R is held opens the zoomed map. Japanese tests the held
   keys, where R is tested first and the map screen opens instead. */
#ifndef WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY
#define WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY 0
#endif

/* Checking the wreck of Babi's ship off Gateway Cave on the World Map shows its
   message without freezing the World Map first and unfreezing it afterwards.
   While Isaac holds the Black Orb, the field menu still opens after the
   message. The other editions freeze the map around the message: a still image
   with the camera paused. */
#ifndef WRECK_SEARCH_WITHOUT_FREEZE
#define WRECK_SEARCH_WITHOUT_FREEZE 0
#endif

/* When a glyph sprite is drawn 0 pixels wide, the glyph drawer returns a width
   of 1, so a zero-width glyph is not mistaken for 'no window sprite free',
   which also returns 0. */
#ifndef ZERO_WIDTH_GLYPH_ADVANCES
#define ZERO_WIDTH_GLYPH_ADVANCES 0
#endif

#endif
