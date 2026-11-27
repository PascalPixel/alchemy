#ifndef ALCHEMY_EDITION_H
#define ALCHEMY_EDITION_H

/* The edition being built: its language, and the features it turns on. The code
   asks for a feature by name, #if FEATURE for a block and WHEN(FEATURE, on,
   off) for a value, and picks what the language decides with JAPANESE_OR,
   BY_LANGUAGE and the like (features.h). Every difference between editions that
   is not the language itself is a feature, turned on here for the editions that
   have it; every repair of the original's bugs and undefined behaviour is a fix
   (fixes.h), off in every edition and turned on by BUGFIX or UBFIX. */

/* The build defines one TLA_EDITION_ name, as 1; none at all is Japanese. */
#if (defined(TLA_EDITION_JA) + defined(TLA_EDITION_EN) + \
	defined(TLA_EDITION_DE) + defined(TLA_EDITION_ES) + \
	defined(TLA_EDITION_FR) + defined(TLA_EDITION_IT)) > 1
#error Select exactly one TLA edition
#endif
#if (defined(TLA_EDITION_JA) && TLA_EDITION_JA != 1) || \
	(defined(TLA_EDITION_EN) && TLA_EDITION_EN != 1) || \
	(defined(TLA_EDITION_DE) && TLA_EDITION_DE != 1) || \
	(defined(TLA_EDITION_ES) && TLA_EDITION_ES != 1) || \
	(defined(TLA_EDITION_FR) && TLA_EDITION_FR != 1) || \
	(defined(TLA_EDITION_IT) && TLA_EDITION_IT != 1)
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

/* The edition's language, from the TLA_EDITION_ the build defines. */
#if defined(TLA_EDITION_EN)
#define LANGUAGE LANGUAGE_ENGLISH
#elif defined(TLA_EDITION_DE)
#define LANGUAGE LANGUAGE_GERMAN
#elif defined(TLA_EDITION_ES)
#define LANGUAGE LANGUAGE_SPANISH
#elif defined(TLA_EDITION_FR)
#define LANGUAGE LANGUAGE_FRENCH
#elif defined(TLA_EDITION_IT)
#define LANGUAGE LANGUAGE_ITALIAN
#else
#define LANGUAGE LANGUAGE_JAPANESE
#endif

#include "features.h"
#include "fixes.h"

/* The features each edition turns on. The catalogue below says what each one
   does. */
#if defined(TLA_EDITION_JA)
#define ATTEKA_ISLET_REBUILD_KEEPS_CRATE_CELL 1
#elif defined(TLA_EDITION_EN)
#define ACHING_BACK_MAN_STAYS_PUT						1
#define AERIE_PARTY_CLEARS_SHORTCUTS					1
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION		1
#define ANEMOS_HOVER_CENTRES_LEADER						1
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED	1
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP				1
#define ARTIFACT_STOCK_SKIPS_EMPTY						1
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR			1
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES	1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 1
#define BEACON_ORB_NULL_CHECK							1
#define BEACON_PARTICLE_SPARE_WORD						1
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER				1
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER			1
#define CRAWL_PAUSE_WITHOUT_SLIDE						1
#define CREDITS_REVISED_FOR_LOCALIZATION				1
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY					1
#define CYCLONE_CLEARS_OSENIA_WEEDS						1
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN			1
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE		1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			1
#define DEFEAT_SCENE_RESTART_MOVES_SHIP					1
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY					1
#define DJINNI_HOP_RESTORES_GRAVITY						1
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT					1
#define DJINNI_LEAP_HIGH_JUMP_SOUND						1
#define DJINNI_SPIN_JUMP_SOUNDS							1
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW				1
#define DJINN_PAGE_WAIT_AFTER_DRAW						1
#define DRAGON_ICE_CRACK_CHECKS_BURST					1
#define DRAGON_STATUE_WAITS_FOR_THAW					1
#define EQUIPMENT_PAGE_REDRAWS_HELP						1
#define EXTRA_BUTTON_CODE_PADDING						1
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL				1
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY				1
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW					1
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT			1
#define TICK_WAIT_BEFORE_PSYNERGY						1
#define FUGUE_NEW_POSITION								1
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR		1
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM				1
#define GALE_TALK_NEEDS_LEADER_FACING_EAST				1
#define GARET_HANG_HEIGHT_ABSOLUTE						1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			1
#define HALTED_ACTOR_CANNOT_BE_PUSHED					1
#define HALT_ON_MARS_AERIE_SPEAKERS						1
#define HI_LO_GAMES_CANNOT_BE_QUIT						1
#define ICE_SLIDE_ENDS_CLEANLY							1
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT					1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					1
#define ITEM_MENU_EXTRA_TEXT_TILES						1
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT				1
#define IZUMO_RUINS_DESCENT_SOUND						1
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED				1
#define KNOCK_BACK_STOPS_AT_ACTORS						1
#define LARGER_PASSWORD_BUFFER							1
#define LEMURIA_DOCK_CANNON_ON_DECK						1
#define LEMURIA_SPRING_PAYS_ON_CONFIRM					1
#define LIFT_RESTORES_LEADER_PRIORITY					1
#define LINE_SPACING_COUNTS_ALWAYS_STORED				1
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION			1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					1
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK			1
#define MADRA_INNKEEPER_YES_NO_QUESTION					1
#define MAP_SCREEN_NAME_DELAYED_REDRAW					1
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER			1
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE		1
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR		1
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE		1
#define MARS_STAR_REUSE_SHORT_SCENE						1
#define MAX_WIDTH_CLEAR_FIRST							1
#define MIND_READ_WINDOW_NOT_WIDENED					1
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS				1
#define NEW_GAME_SETS_RESTART_POINT						1
#define PARTY_SWAP_WAITS_FOR_PREVIEW					1
#define PASSWORD_LEVEL_SETS_EXPERIENCE					1
#define PAUSE_HEADING_LOWER								1
#define PILLAR_PUDDLES_LOW_PRIORITY						1
#define PIT_FALL_DUST_LANDING							1
#define PLATFORM_STEP_TESTS_BOTH_SIDES					1
#define POI_MEETING_RESTORES_HALL_TRANSITION			1
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE		1
#define REVISED_ENTRANCE_TRANSITIONS					1
#define RISING_DRAGON_SHORTER_KNOCKBACK					1
#define SCOOP_DIGS_UP_CRYSTAL_ONCE						1
#define SETTINGS_SHOW_PAUSE_HEADING						1
#define SHOP_TEST_GIVES_EVERY_DJINNI					1
#define SUNSHINE_BED_SILENT_WHILE_FORGING				1
#define TAOPO_CHEST_ROCK_JUMP							1
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY				1
#define TRIAL_ROAD_TAKES_STAT_ITEMS						1
#define UPDATE_KEEPS_PARTY_POSITION						1
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS				1
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS			1
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL			1
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR			1
#define ZERO_WIDTH_GLYPH_ADVANCES						1
#elif defined(TLA_EDITION_DE)
#define ACHING_BACK_MAN_STAYS_PUT						1
#define AERIE_PARTY_CLEARS_SHORTCUTS					1
#define AFTER_ITEM_EARLY_CLEAR							1
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION		1
#define ANEMOS_HOVER_CENTRES_LEADER						1
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED	1
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP				1
#define ARTIFACT_STOCK_SKIPS_EMPTY						1
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR			1
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES	1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 1
#define BEACON_ORB_NULL_CHECK							1
#define BEACON_PARTICLE_SPARE_WORD						1
#define BLAZE_TABLET_TOUCH_LINE_FIXED					1
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER				1
#define BOB_DECK_DEAD_CALL_DROPPED						1
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER			1
#define CRAWL_PAUSE_WITHOUT_SLIDE						1
#define CREDITS_REVISED_FOR_LOCALIZATION				1
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY					1
#define CYCLONE_CLEARS_OSENIA_WEEDS						1
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN			1
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE		1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			1
#define DEFEAT_SCENE_RESTART_MOVES_SHIP					1
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY					1
#define DJINNI_HOP_RESTORES_GRAVITY						1
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT					1
#define DJINNI_LEAP_HIGH_JUMP_SOUND						1
#define DJINNI_SPIN_JUMP_SOUNDS							1
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW				1
#define DJINN_PAGE_WAIT_AFTER_DRAW						1
#define DRAGON_ICE_CRACK_CHECKS_BURST					1
#define DRAGON_ICE_STAYS_BROKEN							1
#define DRAGON_STATUE_WAITS_FOR_THAW					1
#define EQUIPMENT_PAGE_REDRAWS_HELP						1
#define EXTRA_BUTTON_CODE_PADDING						1
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL				1
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY				1
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW					1
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT			1
#define TICK_WAIT_BEFORE_PSYNERGY						1
#define FUGUE_NEW_POSITION								1
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR		1
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM				1
#define GALE_TALK_NEEDS_LEADER_FACING_EAST				1
#define GARET_HANG_HEIGHT_ABSOLUTE						1
#define GIFT_TO_MEMBER_AFTER_DROP_WAITS_FOR_BUTTON		1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			1
#define HALTED_ACTOR_CANNOT_BE_PUSHED					1
#define HALT_ON_MARS_AERIE_SPEAKERS						1
#define HI_LO_GAMES_CANNOT_BE_QUIT						1
#define ICE_SLIDE_ENDS_CLEANLY							1
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT					1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					1
#define ITEM_MENU_EXTRA_TEXT_TILES						1
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT				1
#define IZUMO_RUINS_DESCENT_SOUND						1
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED				1
#define KNOCK_BACK_STOPS_AT_ACTORS						1
#define LARGER_PASSWORD_BUFFER							1
#define LEMURIA_DOCK_CANNON_ON_DECK						1
#define LEMURIA_SPRING_PAYS_ON_CONFIRM					1
#define LIFT_RESTORES_LEADER_PRIORITY					1
#define LINE_SPACING_COUNTS_ALWAYS_STORED				1
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION			1
#define LONG_BATTLE_ITEM_REFUSAL						1
#define LONG_STATUS_ITEM_HELP							1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					1
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK			1
#define MADRA_INNKEEPER_YES_NO_QUESTION					1
#define MAP_SCREEN_NAME_DELAYED_REDRAW					1
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER			1
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE		1
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR		1
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE		1
#define MARS_STAR_REUSE_SHORT_SCENE						1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			1
#define MIND_READ_TEXT_NOT_INDENTED						1
#define MIND_READ_WINDOW_NOT_WIDENED					1
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS				1
#define NEW_GAME_IGNORES_PASSWORD_CANCEL				1
#define NEW_GAME_SETS_RESTART_POINT						1
#define NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME				1
#define PARTY_SWAP_WAITS_FOR_PREVIEW					1
#define PASSWORD_LEVEL_SETS_EXPERIENCE					1
#define PAUSE_HEADING_LOWER								1
#define PILLAR_PUDDLES_LOW_PRIORITY						1
#define PIT_FALL_DUST_LANDING							1
#define PLATFORM_STEP_TESTS_BOTH_SIDES					1
#define POI_MEETING_RESTORES_HALL_TRANSITION			1
#define PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION			1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					1
#define PSYNERGY_PAGE_REDRAWS_HELP						1
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE		1
#define RAGGED_LINES									1
#define REVISED_ENTRANCE_TRANSITIONS					1
#define RISING_DRAGON_SHORTER_KNOCKBACK					1
#define SCOOP_DIGS_UP_CRYSTAL_ONCE						1
#define SETTINGS_SHOW_PAUSE_HEADING						1
#define SHOP_TEST_GIVES_EVERY_DJINNI					1
#define SPACED_ENEMY_NAME_PREFIX						1
#define SUNSHINE_BED_SILENT_WHILE_FORGING				1
#define SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP				1
#define TAOPO_CHEST_ROCK_JUMP							1
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY				1
#define TRIAL_ROAD_TAKES_STAT_ITEMS						1
#define UPDATE_KEEPS_PARTY_POSITION						1
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS				1
#define WIDE_STRING_FIXED_WIDTH							1
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS			1
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL			1
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR			1
#define YEPP_SONGS_VILLAGER_FURTHER_SOUTH				1
#define ZERO_WIDTH_GLYPH_ADVANCES						1
#elif defined(TLA_EDITION_ES)
#define ACHING_BACK_MAN_STAYS_PUT						1
#define AERIE_PARTY_CLEARS_SHORTCUTS					1
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION		1
#define ANEMOS_HOVER_CENTRES_LEADER						1
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED	1
#define ANKOHL_PIT_FALL_DOES_NOT_REPEAT					1
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP				1
#define ARTIFACT_STOCK_SKIPS_EMPTY						1
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR			1
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES	1
#define BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI	1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 1
#define BEACON_ORB_NULL_CHECK							1
#define BEACON_PARTICLE_SPARE_WORD						1
#define BLAZE_TABLET_TOUCH_LINE_FIXED					1
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER				1
#define BOB_DECK_DEAD_CALL_DROPPED						1
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER			1
#define CRAWL_PAUSE_WITHOUT_SLIDE						1
#define CREDITS_REVISED_FOR_LOCALIZATION				1
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY					1
#define CYCLONE_CLEARS_OSENIA_WEEDS						1
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN			1
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE		1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			1
#define DEFEAT_SCENE_RESTART_MOVES_SHIP					1
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY					1
#define DJINNI_HOP_RESTORES_GRAVITY						1
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT					1
#define DJINNI_LEAP_HIGH_JUMP_SOUND						1
#define DJINNI_SPIN_JUMP_SOUNDS							1
#define DJINN_COLUMN_WAIT_AFTER_DRAW					1
#define DJINN_EFFECT_CLEARED_WITHOUT_DJINNI				1
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW				1
#define DJINN_PAGE_WAIT_AFTER_DRAW						1
#define DRAGON_ICE_CRACK_CHECKS_BURST					1
#define DRAGON_ICE_STAYS_BROKEN							1
#define DRAGON_STATUE_WAITS_FOR_THAW					1
#define EQUIPMENT_PAGE_REDRAWS_HELP						1
#define EXTRA_BUTTON_CODE_PADDING						1
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL				1
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY				1
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW					1
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT			1
#define TICK_WAIT_BEFORE_PSYNERGY						1
#define FUGUE_NEW_POSITION								1
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR		1
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM				1
#define GALE_TALK_NEEDS_LEADER_FACING_EAST				1
#define GARET_HANG_HEIGHT_ABSOLUTE						1
#define GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN				1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			1
#define HALTED_ACTOR_CANNOT_BE_PUSHED					1
#define HALT_ON_MARS_AERIE_SPEAKERS						1
#define HI_LO_GAMES_CANNOT_BE_QUIT						1
#define ICE_SLIDE_ENDS_CLEANLY							1
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT					1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					1
#define ITEM_MENU_EXTRA_TEXT_TILES						1
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT				1
#define IZUMO_RUINS_DESCENT_SOUND						1
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED				1
#define KNOCK_BACK_STOPS_AT_ACTORS						1
#define LARGER_PASSWORD_BUFFER							1
#define LEMURIA_DOCK_CANNON_ON_DECK						1
#define LEMURIA_SPRING_PAYS_ON_CONFIRM					1
#define LIFT_RESTORES_LEADER_PRIORITY					1
#define LINE_SPACING_COUNTS_ALWAYS_STORED				1
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION			1
#define LONG_BATTLE_ITEM_REFUSAL						1
#define LONG_STATUS_DJINN_HELP							1
#define LONG_STATUS_ITEM_HELP							1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					1
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK			1
#define MADRA_INNKEEPER_YES_NO_QUESTION					1
#define MAP_SCREEN_NAME_DELAYED_REDRAW					1
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER			1
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE		1
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR		1
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE		1
#define MARS_STAR_REUSE_SHORT_SCENE						1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			1
#define MIND_READ_TEXT_NOT_INDENTED						1
#define MIND_READ_WINDOW_NOT_WIDENED					1
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS				1
#define NEW_GAME_IGNORES_PASSWORD_CANCEL				1
#define NEW_GAME_SETS_RESTART_POINT						1
#define NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME				1
#define OUTLINED_GLYPH_PAIRS_FIT_CELL					1
#define PARTY_SWAP_WAITS_FOR_PREVIEW					1
#define PASSWORD_LEVEL_SETS_EXPERIENCE					1
#define PAUSE_HEADING_LOWER								1
#define PILLAR_PUDDLES_LOW_PRIORITY						1
#define PIT_FALL_DUST_LANDING							1
#define PLATFORM_STEP_TESTS_BOTH_SIDES					1
#define POI_MEETING_RESTORES_HALL_TRANSITION			1
#define PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION			1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					1
#define PSYNERGY_PAGE_REDRAWS_HELP						1
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE		1
#define REVISED_ENTRANCE_TRANSITIONS					1
#define RISING_DRAGON_SHORTER_KNOCKBACK					1
#define SCOOP_DIGS_UP_CRYSTAL_ONCE						1
#define SETTINGS_SHOW_PAUSE_HEADING						1
#define SHOP_TEST_GIVES_EVERY_DJINNI					1
#define SPACED_ENEMY_NAME_PREFIX						1
#define STATUS_DJINN_TURNS_HIDDEN						1
#define SUNSHINE_BED_SILENT_WHILE_FORGING				1
#define SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP				1
#define TAOPO_CHEST_ROCK_JUMP							1
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY				1
#define TRANSFERRED_COINS_CAPPED						1
#define TRIAL_ROAD_TAKES_STAT_ITEMS						1
#define UPDATE_KEEPS_PARTY_POSITION						1
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS				1
#define WIDE_STRING_FIXED_WIDTH							1
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS			1
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL			1
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR			1
#define YEPP_SONGS_VILLAGER_FURTHER_SOUTH				1
#define ZERO_WIDTH_GLYPH_ADVANCES						1
#elif defined(TLA_EDITION_FR)
#define ACHING_BACK_MAN_STAYS_PUT						1
#define AERIE_PARTY_CLEARS_SHORTCUTS					1
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION		1
#define ANEMOS_HOVER_CENTRES_LEADER						1
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED	1
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP				1
#define ARTIFACT_STOCK_SKIPS_EMPTY						1
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR			1
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES	1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 1
#define BEACON_ORB_NULL_CHECK							1
#define BEACON_PARTICLE_SPARE_WORD						1
#define BLAZE_TABLET_TOUCH_LINE_FIXED					1
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER				1
#define BOB_DECK_DEAD_CALL_DROPPED						1
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER			1
#define CRAWL_PAUSE_WITHOUT_SLIDE						1
#define CREDITS_REVISED_FOR_LOCALIZATION				1
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY					1
#define CYCLONE_CLEARS_OSENIA_WEEDS						1
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN			1
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE		1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			1
#define DEFEAT_SCENE_RESTART_MOVES_SHIP					1
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY					1
#define DJINNI_HOP_RESTORES_GRAVITY						1
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT					1
#define DJINNI_LEAP_HIGH_JUMP_SOUND						1
#define DJINNI_SPIN_JUMP_SOUNDS							1
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW				1
#define DJINN_PAGE_WAIT_AFTER_DRAW						1
#define DRAGON_ICE_CRACK_CHECKS_BURST					1
#define DRAGON_ICE_STAYS_BROKEN							1
#define DRAGON_STATUE_WAITS_FOR_THAW					1
#define EQUIPMENT_PAGE_REDRAWS_HELP						1
#define EXTRA_BUTTON_CODE_PADDING						1
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL				1
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY				1
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW					1
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT			1
#define TICK_WAIT_BEFORE_PSYNERGY						1
#define FUGUE_NEW_POSITION								1
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR		1
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM				1
#define GALE_TALK_NEEDS_LEADER_FACING_EAST				1
#define GARET_HANG_HEIGHT_ABSOLUTE						1
#define GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN				1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			1
#define HALTED_ACTOR_CANNOT_BE_PUSHED					1
#define HALT_ON_MARS_AERIE_SPEAKERS						1
#define HI_LO_GAMES_CANNOT_BE_QUIT						1
#define ICE_SLIDE_ENDS_CLEANLY							1
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT					1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					1
#define ITEM_MENU_EXTRA_TEXT_TILES						1
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT				1
#define IZUMO_RUINS_DESCENT_SOUND						1
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED				1
#define KNOCK_BACK_STOPS_AT_ACTORS						1
#define LARGER_PASSWORD_BUFFER							1
#define LEMURIA_DOCK_CANNON_ON_DECK						1
#define LEMURIA_SPRING_PAYS_ON_CONFIRM					1
#define LIFT_RESTORES_LEADER_PRIORITY					1
#define LINE_SPACING_COUNTS_ALWAYS_STORED				1
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION			1
#define LONG_BATTLE_ITEM_REFUSAL						1
#define LONG_STATUS_DJINN_HELP							1
#define LONG_STATUS_ITEM_HELP							1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					1
#define LUCKY_WHEELS_THREE_DIGIT_TICKET_COUNT			1
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK			1
#define MADRA_INNKEEPER_YES_NO_QUESTION					1
#define MAP_SCREEN_NAME_DELAYED_REDRAW					1
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER			1
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE		1
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR		1
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE		1
#define MARS_STAR_REUSE_SHORT_SCENE						1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			1
#define MIND_READ_TEXT_NOT_INDENTED						1
#define MIND_READ_WINDOW_NOT_WIDENED					1
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS				1
#define NEW_GAME_IGNORES_PASSWORD_CANCEL				1
#define NEW_GAME_SETS_RESTART_POINT						1
#define NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME				1
#define PARTY_SWAP_WAITS_FOR_PREVIEW					1
#define PASSWORD_LEVEL_SETS_EXPERIENCE					1
#define PAUSE_HEADING_LOWER								1
#define PILLAR_PUDDLES_LOW_PRIORITY						1
#define PIT_FALL_DUST_LANDING							1
#define PLATFORM_STEP_TESTS_BOTH_SIDES					1
#define POI_MEETING_RESTORES_HALL_TRANSITION			1
#define PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION			1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					1
#define PSYNERGY_PAGE_REDRAWS_HELP						1
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE		1
#define REVISED_ENTRANCE_TRANSITIONS					1
#define RISING_DRAGON_SHORTER_KNOCKBACK					1
#define SCOOP_DIGS_UP_CRYSTAL_ONCE						1
#define SETTINGS_SHOW_PAUSE_HEADING						1
#define SHOP_TEST_GIVES_EVERY_DJINNI					1
#define SPACED_ENEMY_NAME_PREFIX						1
#define SUNSHINE_BED_SILENT_WHILE_FORGING				1
#define SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP				1
#define TAOPO_CHEST_ROCK_JUMP							1
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY				1
#define TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW			1
#define TRIAL_ROAD_TAKES_STAT_ITEMS						1
#define UNUSED_HP_PP_LABEL_TILES_DROPPED				1
#define UPDATE_KEEPS_PARTY_POSITION						1
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS				1
#define WIDE_STRING_FIXED_WIDTH							1
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS			1
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL			1
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR			1
#define YEPP_SONGS_VILLAGER_FURTHER_SOUTH				1
#define ZERO_WIDTH_GLYPH_ADVANCES						1
#elif defined(TLA_EDITION_IT)
#define ACHING_BACK_MAN_STAYS_PUT						1
#define AERIE_PARTY_CLEARS_SHORTCUTS					1
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION		1
#define ANEMOS_HOVER_CENTRES_LEADER						1
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED	1
#define ANKOHL_PIT_FALL_DOES_NOT_REPEAT					1
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP				1
#define ARTIFACT_STOCK_SKIPS_EMPTY						1
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR			1
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES	1
#define BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI	1
#define BATTLE_LABELS_BY_HEADER_LANGUAGE				1
#define BATTLE_STATUS_FREES_UNSHOWN_TEXT_TILES_ON_CLOSE 1
#define BEACON_ORB_NULL_CHECK							1
#define BEACON_PARTICLE_SPARE_WORD						1
#define BLAZE_TABLET_TOUCH_LINE_FIXED					1
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER				1
#define BOB_DECK_DEAD_CALL_DROPPED						1
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER			1
#define CRAWL_PAUSE_WITHOUT_SLIDE						1
#define CREDITS_REVISED_FOR_LOCALIZATION				1
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY					1
#define CYCLONE_CLEARS_OSENIA_WEEDS						1
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN			1
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE		1
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET			1
#define DEFEAT_SCENE_RESTART_MOVES_SHIP					1
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY					1
#define DJINNI_HOP_RESTORES_GRAVITY						1
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT					1
#define DJINNI_LEAP_HIGH_JUMP_SOUND						1
#define DJINNI_SPIN_JUMP_SOUNDS							1
#define DJINN_COLUMN_WAIT_AFTER_DRAW					1
#define DJINN_EFFECT_CLEARED_WITHOUT_DJINNI				1
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW				1
#define DJINN_PAGE_WAIT_AFTER_DRAW						1
#define DRAGON_ICE_CRACK_CHECKS_BURST					1
#define DRAGON_ICE_STAYS_BROKEN							1
#define DRAGON_STATUE_WAITS_FOR_THAW					1
#define EQUIPMENT_PAGE_REDRAWS_HELP						1
#define EXTRA_BUTTON_CODE_PADDING						1
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL				1
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY				1
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW					1
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT			1
#define TICK_WAIT_BEFORE_PSYNERGY						1
#define FUGUE_NEW_POSITION								1
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR		1
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM				1
#define GALE_TALK_NEEDS_LEADER_FACING_EAST				1
#define GARET_HANG_HEIGHT_ABSOLUTE						1
#define GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN				1
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED			1
#define HALTED_ACTOR_CANNOT_BE_PUSHED					1
#define HALT_ON_MARS_AERIE_SPEAKERS						1
#define HI_LO_GAMES_CANNOT_BE_QUIT						1
#define ICE_SLIDE_ENDS_CLEANLY							1
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT					1
#define ITEM_MENU_CLOSES_MESSAGE_WINDOW					1
#define ITEM_MENU_EXTRA_TEXT_TILES						1
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT				1
#define IZUMO_RUINS_DESCENT_SOUND						1
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED				1
#define KNOCK_BACK_STOPS_AT_ACTORS						1
#define LARGER_PASSWORD_BUFFER							1
#define LEMURIA_DOCK_CANNON_ON_DECK						1
#define LEMURIA_SPRING_PAYS_ON_CONFIRM					1
#define LIFT_RESTORES_LEADER_PRIORITY					1
#define LINE_SPACING_COUNTS_ALWAYS_STORED				1
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION			1
#define LONG_BATTLE_ITEM_REFUSAL						1
#define LONG_STATUS_DJINN_HELP							1
#define LONG_STATUS_ITEM_HELP							1
#define LUCKY_WHEELS_PROMPT_DRAWN_ONCE					1
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK			1
#define MADRA_INNKEEPER_YES_NO_QUESTION					1
#define MAP_SCREEN_NAME_DELAYED_REDRAW					1
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER			1
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE		1
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR		1
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE		1
#define MARS_STAR_REUSE_SHORT_SCENE						1
#define MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER			1
#define MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER			1
#define MIND_READ_TEXT_NOT_INDENTED						1
#define MIND_READ_WINDOW_NOT_WIDENED					1
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS				1
#define NEW_GAME_IGNORES_PASSWORD_CANCEL				1
#define NEW_GAME_SETS_RESTART_POINT						1
#define NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME				1
#define OUTLINED_GLYPH_PAIRS_FIT_CELL					1
#define PARTY_SWAP_WAITS_FOR_PREVIEW					1
#define PASSWORD_LEVEL_SETS_EXPERIENCE					1
#define PAUSE_HEADING_LOWER								1
#define PILLAR_PUDDLES_LOW_PRIORITY						1
#define PIT_FALL_DUST_LANDING							1
#define PLATFORM_STEP_TESTS_BOTH_SIDES					1
#define POI_MEETING_RESTORES_HALL_TRANSITION			1
#define PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION			1
#define PSYNERGY_MENU_EXTRA_TEXT_TILES					1
#define PSYNERGY_PAGE_REDRAWS_HELP						1
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE		1
#define REVISED_ENTRANCE_TRANSITIONS					1
#define RISING_DRAGON_SHORTER_KNOCKBACK					1
#define SCOOP_DIGS_UP_CRYSTAL_ONCE						1
#define SETTINGS_SHOW_PAUSE_HEADING						1
#define SHOP_TEST_GIVES_EVERY_DJINNI					1
#define SPACED_ENEMY_NAME_PREFIX						1
#define STATUS_DJINN_TURNS_HIDDEN						1
#define SUNSHINE_BED_SILENT_WHILE_FORGING				1
#define SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP				1
#define TAOPO_CHEST_ROCK_JUMP							1
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY				1
#define TRANSFERRED_COINS_CAPPED						1
#define TRIAL_ROAD_TAKES_STAT_ITEMS						1
#define UPDATE_KEEPS_PARTY_POSITION						1
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS				1
#define WIDE_STRING_FIXED_WIDTH							1
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS			1
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL			1
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR			1
#define YEPP_SONGS_VILLAGER_FURTHER_SOUTH				1
#define ZERO_WIDTH_GLYPH_ADVANCES						1
#endif

/* The features, each off unless an edition turns it on above. */

/* In Kibombo's houses, when the scene comes back with kept positions (from
   battle or a menu), the old man lying in bed with an aching back
   (ACTOR_MAN_WITH_ACHING_BACK) is moved back 14 and 8 pixels before he is laid
   down and shifted again, so he stays where he lies. In Japanese the shift adds
   up on every such return, so he and the wall cells under him creep across the
   room. */
#ifndef ACHING_BACK_MAN_STAYS_PUT
#define ACHING_BACK_MAN_STAYS_PUT 0
#endif

/* When the others stay behind on the Jupiter Lighthouse terraces and Piers
   joins Felix for the aerie, any button shortcut whose Psynergy the two of them
   cannot cast is cleared at once. In Japanese those shortcuts stay until the
   first full step of free walking, when the unconditional per-step clear
   removes them. */
#ifndef AERIE_PARTY_CLEARS_SHORTCUTS
#define AERIE_PARTY_CLEARS_SHORTCUTS 0
#endif

/* German also clears the after-item-name marker of
   NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME at the very start of decoding. Nothing
   reads the marker before the first code clears it again, so only the compiled
   code differs. */
#ifndef AFTER_ITEM_EARLY_CLEAR
#define AFTER_ITEM_EARLY_CLEAR 0
#endif

/* Entering the Air's Rock hall from the Reveal tablet room moves the leader to
   layer 1 and recentres the camera only when the party's position is not being
   kept (FLAG_KEEP_PARTY_POSITION clear, as it is except after a battle). The
   Japanese edition always does it, even when the position is kept. */
#ifndef AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION
#define AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION 0
#endif

/* When Felix casts Hover on the Anemos Inner Sanctum entrance's six floating
   blocks, the leader is first sent to the centre of the cell he stands on, at
   his current height, before he turns and the blocks lift. The Japanese edition
   leaves him where he stood. */
#ifndef ANEMOS_HOVER_CENTRES_LEADER
#define ANEMOS_HOVER_CENTRES_LEADER 0
#endif

/* In Ankohl Ruins, the half face can be pushed into a floor gap with Move. In
   the Japanese release, Move opens the gap's cells before every push of the
   half face and closes them after it. From the later editions, Move opens them
   only while Reveal shows the gap, and Reveal's fading closes them. A new flag,
   FLAG_ANKOHL_RUINS_REVEAL_SHOWN, tracks whether the gap is shown: Reveal sets
   it and the fading clears it. */
#ifndef ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED
#define ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED 0
#endif

/* After Ankohl Ruins' pit landing scene, FLAG_ARRIVE_FALLING is cleared. The
   landing scene opens the screen itself, so the field never plays the generic
   arrival (fdEntrancePlayArrival), which is the only code that clears that
   flag. Without this line (en, de, fr) the flag stays set, and the next
   ordinary arrival drops the leader in from 160 pixels again: the next room, or
   this room on return from a battle. */
#ifndef ANKOHL_PIT_FALL_DOES_NOT_REPEAT
#define ANKOHL_PIT_FALL_DOES_NOT_REPEAT 0
#endif

/* In Aqua Rock, the stone pillar pushed east off the ledge in Steam's room
   falls inside an event that locks input and ends lasting Psynergy. It waits
   until the pillar has actually landed, instead of a fixed 15 ticks, then
   plays SE_THUMP before the dropped flag and its hop-across cell are set. */
#ifndef AQUA_ROCK_PILLAR_LANDS_WITH_THUMP
#define AQUA_ROCK_PILLAR_LANDS_WITH_THUMP 0
#endif

/* On a shop's first visit, every nonzero entry in the artifact columns of its
   stock row is added to the shared stock, and empty entries are skipped.
   Japanese stops at the row's first empty entry. So in Japanese, the entries
   after a gap in the Shaman Village, Loho and Prox general stores (rows 29 to
   31: Vial, Potion, Mist Potion, Psy Crystal, Water of Life) are never
   stocked. */
#ifndef ARTIFACT_STOCK_SKIPS_EMPTY
#define ARTIFACT_STOCK_SKIPS_EMPTY 0
#endif

/* When SW Atteka Islet is rebuilt with its scene flags kept and the falls crate
   has come to rest against the sunk pillar, cell (11, 11) gets its walkable
   attributes back, as scSettlementsSlideCrateDownFalls set them. This is part
   of the path to the Dragon Skin's chest. The later editions dropped this line
   and restore nothing at (11, 11). */
#ifndef ATTEKA_ISLET_REBUILD_KEEPS_CRATE_CELL
#define ATTEKA_ISLET_REBUILD_KEEPS_CRATE_CELL 0
#endif

/* When SW Atteka Islet is rebuilt with its scene flags kept (for example on
   return from a battle or a menu) and the pillar has been pushed into the
   river, the pillar stays out of sight, river stone K stands in its place and
   cell (11, 12) stays walkable. This is done in scInit and again in
   scPrepareMap. The Japanese edition does not restore it. An ordinary entry
   clears FLAG_PILLAR_SUNK and resets the puzzle in every edition. */
#ifndef ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR
#define ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR 0
#endif

/* In the battle Djinn menu, when the preview leaves its Psynergy pages and goes
   back to the stats page (the branch where previewPage != 0 is reset to 0), the
   Djinn list is redrawn. Before, the list was redrawn only when its first entry
   changed. Outside Japanese, the 21-tile Psynergy pages window covers the
   list's left column, so without the redraw that column stays covered. */
#ifndef BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
#define BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES 0
#endif

/* In the battle Djinn menu, the cursor can move onto a Djinni already chosen
   this turn or still recovering, and the open preview then closes. When that
   happens, the Djinn list is also redrawn, so the column the Psynergy pages
   window covered does not stay blank. */
#ifndef BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI
#define BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI 0
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

/* The per-tick task that pulses the lit Jupiter Lighthouse beacon's light
   returns early when the orb actor (ACTOR_BEACON_ORB) is missing. Japanese goes
   on and reads the missing actor's sprite through a null pointer. */
#ifndef BEACON_ORB_NULL_CHECK
#define BEACON_ORB_NULL_CHECK 0
#endif

/* The script for the beacon's rising light particles carries one extra 0 word
   after its first SC_RELEASE, in the tail no code ever runs. It changes only
   the data layout: that unused tail and the LATE_DATA after it move by 4 bytes,
   and nothing the player sees changes. */
#ifndef BEACON_PARTICLE_SPARE_WORD
#define BEACON_PARTICLE_SPARE_WORD 0
#endif

/* As Jenna lays her hands on the Blaze tablet in Magma Rock, the game shows the
   'touched the tablet' line. English dropped the blank placeholder that the
   Japanese catalogue has at +2 but kept the Japanese index +3, so the English
   ROM shows 'received the fire Psynergy Blaze' before the ground shakes and
   never shows the touched line. From German on, the index is +2, the touched
   line. */
#ifndef BLAZE_TABLET_TOUCH_LINE_FIXED
#define BLAZE_TABLET_TOUCH_LINE_FIXED 0
#endif

/* After the leader jumps aboard the Lemurian ship on the world map, any step
   trigger still pending is cleared, chiefly one on a cell crossed during the
   leap. In Japanese that step trigger still runs after the party lands on the
   ship. */
#ifndef BOARDING_SHIP_CLEARS_STEP_TRIGGER
#define BOARDING_SHIP_CLEARS_STEP_TRIGGER 0
#endif

/* The dead if(0) call that would start the deck bob task, at the top of the
   whirlpool scene at the tiller (scLemurianShipDeckHoldOnToTiller, entrance
   20), is gone. It never ran, so only the compiled code differs. The deck still
   bobs, started later in the same function in every edition. */
#ifndef BOB_DECK_DEAD_CALL_DROPPED
#define BOB_DECK_DEAD_CALL_DROPPED 0
#endif

/* A child playing a walking game, the game of chase in Prox after the ending or
   the two children running their rings in Yallam, is set walking (unpaused)
   when the leader is within 11 pixels, that is, overlapping him. That skips
   every hold-still test, so the child walks out of the leader instead of
   freezing inside him. In Japanese such a child can stand still against the
   leader, while the leader is ahead of it within its range, the child it
   follows is ahead of it, or it is being talked to. */
#ifndef CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER
#define CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER 0
#endif

/* In the intro story crawl, a line of four dashes ("----") pauses the crawl
   until A or B is pressed, and the current picture stays up (no next slide
   comes in). window_messages.c:543 makes the dash count return 3 for that line.
   credits.c:414 lets A or B restart the crawl after that stop. In Japanese a
   "----" line counts as three dashes and ends the crawl. Only the German crawl
   uses such lines, four of them (de.po:20302, 20734, 20798, 20966), so the
   comment on CRAWL_STOP_LONG in credits.c, "which no crawl line has", is wrong
   for German. */
#ifndef CRAWL_PAUSE_WITHOUT_SLIDE
#define CRAWL_PAUSE_WITHOUT_SLIDE 0
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

/* In Alhafra's houses, once Isaac has cleared Crossbone Isle, the two sailors
   appear in Alex's old room only when Felix entered the scene from its east
   side (the leader at x 512 or more when it loads). Only then can their
   Crossbone Isle tale be overheard. The Japanese edition shows the sailors and
   plays the tale whichever way he came in. */
#ifndef CROSSBONE_TALE_NEEDS_EAST_ENTRY
#define CROSSBONE_TALE_NEEDS_EAST_ENTRY 0
#endif

/* Cyclone blows away six clumps of weeds on N Osenia Islet: around the cow's
   yard, by the house, south of the yard, on the tilled field, east of it and by
   its fence. The cleared grass is recorded (syFieldTileCopiesCopyTouchedCells),
   so it stays cleared after a battle or the menu. The six events
   (scIsletsClearFirstPatch to scIsletsClearSixthPatch) are new at
   islets.c:1907, and their six Cyclone trigger rows are at islets.c:3081. In
   Japanese, Cyclone does nothing to these weeds. */
#ifndef CYCLONE_CLEARS_OSENIA_WEEDS
#define CYCLONE_CLEARS_OSENIA_WEEDS 0
#endif

/* At a turn's end, the record of which queued Daedalus strikes an enemy
   summoned is cleared after it is copied into the turn's queue. A past enemy
   Daedalus then no longer marks a later party Daedalus strike as the enemies',
   which would play it against the party. */
#ifndef DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN
#define DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN 0
#endif

/* The leader can no longer push three pacing villagers: Tavi's father in his
   house in Daila, the cautious mother in her house on the Apojii Islands
   (PACER_COLLISION), and the man in the green vest on E Tundaria Islet. Each
   script sets its collision without ACTOR_PUSHABLE. daila_rooms.c:873 makes the
   same change in an unused second script that follows SC_END, so it changes
   bytes only. Other pacers keep the bit in every edition, such as the woman in
   Yallam who asks about children's songs. */
#ifndef DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE
#define DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE 0
#endif

/* In the unused battle debug value editor, holding Select returns the later
   editions' preset: 399 instead of 480 in The Broken Seal, and 544 instead of
   411 in The Lost Age. The Lost Age's other preset, held R, is 399 in every
   edition and does not change. */
#ifndef DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET
#define DEBUG_VALUE_EDITOR_NEW_SELECT_PRESET 0
#endif

/* After a defeat, two scenes send the party on to the restart point: Susa's
   scene at the Serpent in Gaia Rock (beaten, or beaten while holding the idol)
   and the gladiators' scene in the Shaman Village cave. Once the ship can sail,
   these scenes also move the ship's world-map mooring to the one for that
   restart point. The Japanese edition leaves the ship where the defeat
   destination's rule put it. */
#ifndef DEFEAT_SCENE_RESTART_MOVES_SHIP
#define DEFEAT_SCENE_RESTART_MOVES_SHIP 0
#endif

/* Six World Map Djinni are met at random: Iron, Sour, Chill, Wheeze, Petra and
   Core. They can now be met only while the party walks the World Map, not
   aboard the ship or the winged ship, as sand, or hovering. In the Japanese
   release they can be met in any movement mode. */
#ifndef DJINNI_ENCOUNTER_ON_FOOT_ONLY
#define DJINNI_ENCOUNTER_ON_FOOT_ONLY 0
#endif

/* At the end of a Djinni's battle-start hop, its gravity is set back to the
   normal 1. The Japanese release leaves it at the quarter gravity used for the
   spinning jumps, so a Djinni put back on the field after the battle falls at
   quarter speed. */
#ifndef DJINNI_HOP_RESTORES_GRAVITY
#define DJINNI_HOP_RESTORES_GRAVITY 0
#endif

/* After a Djinni joins the party, its actor's entry in the field's table of
   actors (FieldState.actors) is cleared. The Japanese release leaves the entry
   set. */
#ifndef DJINNI_JOIN_CLEARS_ACTOR_SLOT
#define DJINNI_JOIN_CLEARS_ACTOR_SLOT 0
#endif

/* Before its battle, a Djinni leaps 16 pixels at the leader. That leap plays
   SE_HIGH_JUMP instead of SE_WHOOSH. */
#ifndef DJINNI_LEAP_HIGH_JUMP_SOUND
#define DJINNI_LEAP_HIGH_JUMP_SOUND 0
#endif

/* Before its battle, a Djinni jumps three times in place, spinning. Each of
   these jumps now plays SE_JUMP; in the Japanese release they are silent. */
#ifndef DJINNI_SPIN_JUMP_SOUNDS
#define DJINNI_SPIN_JUMP_SOUNDS 0
#endif

/* Drawing a Djinn menu page waits one tick after each party member's column,
   for occupied columns only. The tilemap upload is held, so this only adds
   delay and does not show the columns one by one. */
#ifndef DJINN_COLUMN_WAIT_AFTER_DRAW
#define DJINN_COLUMN_WAIT_AFTER_DRAW 0
#endif

/* When the Djinn menu's cursor is on a member's column header rather than a
   Djinni (DJINN_COLUMN_ON_HEADER, selectedRow == -1), the effect line's tiles
   are cleared, so the last Djinni's effect text no longer stays on screen. */
#ifndef DJINN_EFFECT_CLEARED_WITHOUT_DJINNI
#define DJINN_EFFECT_CLEARED_WITHOUT_DJINNI 0
#endif

/* Every redraw of the Djinn menu, a plain cursor move included, waits one tick
   after the help window and another after the trade help line. With the flag
   off, as in Japanese, the menu waits one tick only when it redraws the page,
   after the page arrows. */
#ifndef DJINN_MENU_WAITS_ON_EVERY_REDRAW
#define DJINN_MENU_WAITS_ON_EVERY_REDRAW 0
#endif

/* After a Djinn menu page is drawn, the menu waits three ticks. With the flag
   off it does not wait. */
#ifndef DJINN_PAGE_WAIT_AFTER_DRAW
#define DJINN_PAGE_WAIT_AFTER_DRAW 0
#endif

/* Bursting the Flame Dragons' ice sets the scene flag FLAG_DRAGON_ICE_BURST
   (260). Both fireballs' crack tests then read it next to
   FLAG_FLAME_DRAGONS_BEATEN (in mars_lighthouse_star_room.c, the Japanese test
   against the others). Without DRAGON_ICE_STAYS_BROKEN the two are joined with
   "or", as in en: the crack shows unless the ice is burst and the dragons are
   beaten. Before the battle that is no change from Japanese. After an ordinary
   re-entry the burst flag has been cleared, so Blaze on the west torch brings
   the crack back and sets FLAG_BURST_CAN_BREAK_DRAGON_ICE, which Japanese does
   not do. */
#ifndef DRAGON_ICE_CRACK_CHECKS_BURST
#define DRAGON_ICE_CRACK_CHECKS_BURST 0
#endif

/* The fireballs' crack test joins the burst and dragons-beaten flags with "and"
   instead of "or", so a fireball cracks the Flame Dragons' ice only while the
   ice is unburst and the dragons unbeaten. A fireball still in flight after
   Burst no longer cracks it, and the crack never comes back after the battle.
   Requires DRAGON_ICE_CRACK_CHECKS_BURST. */
#ifndef DRAGON_ICE_STAYS_BROKEN
#define DRAGON_ICE_STAYS_BROKEN 0
#endif

/* When the party holds the Mars Star, the dragon statue in Mars Lighthouse's
   hall gives its "You who hold the star" speech only after the lighthouse has
   thawed (FLAG_MARS_LIGHTHOUSE_THAWED). The speech flashes the four doorways
   and sets FLAG_TOLD_TO_LIGHT_FLAMES. In Japanese the statue speaks at once,
   between taking the star and setting it, so it flashes doorways whose step
   triggers are still off and never speaks again. The "Begone" line for a party
   without the star is the same in every edition. */
#ifndef DRAGON_STATUE_WAITS_FOR_THAW
#define DRAGON_STATUE_WAITS_FOR_THAW 0
#endif

/* The Status screen's equipment page draws its help line again after every
   refresh: on entry, on an L/R character switch, and when the cursor or page
   moves. The Japanese edition draws it only once, before the page's loop. */
#ifndef EQUIPMENT_PAGE_REDRAWS_HELP
#define EQUIPMENT_PAGE_REDRAWS_HELP 0
#endif

/* The long secret button code (ClearExtraButtonCode) has a second 0 after its
   terminating 0. Nothing reads past the first 0, so only the data layout
   changes: the array is 32 bytes instead of 30. */
#ifndef EXTRA_BUTTON_CODE_PADDING
#define EXTRA_BUTTON_CODE_PADDING 0
#endif

/* Once both of Ankohl Ruins' face halves are in place, they chime
   (SE_ANKOHL_RUINS_PIECE) as the party enters their hall. That already happens
   through the hall's other entrances, and now also on arrival down the Crystal
   Powder chest's pit. The entrance range that chimes ends at
   CRYSTAL_POWDER_CHEST_SOUTHWEST (17) instead of SOUTH_B (16). In Japanese the
   pit arrival makes no chime. */
#ifndef FACE_HALVES_CHIME_ON_PIT_ARRIVAL
#define FACE_HALVES_CHIME_ON_PIT_ARRIVAL 0
#endif

/* In Champa, in a game linked from a Broken Seal save, Feizhi's run-up past the
   gravestones (her 'Isaac!', the mistaken identity, Kraden's teasing and the
   Golden Ring) no longer plays once Isaac has joined the party.
   FLAG_FEIZHI_CAME is not set either. */
#ifndef FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY
#define FEIZHI_SKIPPED_WHEN_ISAAC_IN_PARTY 0
#endif

/* Echo's scripted walk through the field menu on the World Map
   (mnAnimateSelectionToEntry, from Psynergy to Djinn) skips the menu's centring
   layout. No label window opens, so no command name is shown while the cursor
   moves, and the icons stay where they were added (tiles 4, 7, 10 and 13
   instead of 7, 10, 13 and 16). Japanese centres the row and opens the label
   window, as the real field menu does in every edition. */
#ifndef FIELD_MENU_DEMO_NO_LABEL_WINDOW
#define FIELD_MENU_DEMO_NO_LABEL_WINDOW 0
#endif

/* Naribwe's fortune-teller reads an equipped Trident as the Trident ("a
   fearsome weapon"). The item is identified from its inventory entry masked to
   the item id (INVENTORY_ITEM_ID_MASK), which drops the equipped, broken and
   quantity bits (466). In Japanese an equipped Trident fails the Trident test,
   because the u16 cast keeps the equipped bit, and gets the reading for other
   items. Sites 348 (the item local, s32 against u32) and 361 (the picker result
   tested directly, not first stored in item) are the same rewrite of the
   function and change only the compiled code. */
#ifndef FORTUNE_TELLER_READS_EQUIPPED_TRIDENT
#define FORTUNE_TELLER_READS_EQUIPPED_TRIDENT 0
#endif

/* Psynergy cast from the field (a request that pays PP) waits one tick after
   the area name is hidden and the World Map is frozen, before the Psynergy
   starts. Requests that pay no PP skip both the freeze and the wait. */
#ifndef TICK_WAIT_BEFORE_PSYNERGY
#define TICK_WAIT_BEFORE_PSYNERGY 0
#endif

/* At the Mars Lighthouse entrance, the Djinni Fugue stands at (728, 408) facing
   north-west. The Japanese edition has it at (664, 424) facing south, with its
   sprite forced to priority 3. */
#ifndef FUGUE_NEW_POSITION
#define FUGUE_NEW_POSITION 0
#endif

/* Climbing from the Gabomba Catacombs into the witch doctor's chamber points
   Retreat back to the door the party last entered the statue by: the hall of
   pinions or the idol's chamber, as FLAG_GABOMBA_RETREAT_TO_PINION / _TO_IDOL
   record. The Japanese edition leaves Retreat at the area-change default, which
   is the witch doctor's chamber itself. */
#ifndef GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR
#define GABOMBA_WITCH_DOCTOR_RETREAT_TO_LAST_DOOR 0
#endif

/* In Gaia Rock, the two stone pillars of the first cliffs play SE_BOOM as they
   land, through the Ankohl Ruins' wait-to-land helper. The Japanese edition's
   helper waits for the landing in the same way (at most 60 ticks, then 10
   more) but plays no sound. There is no event here in any edition. */
#ifndef GAIA_ROCK_PILLAR_LANDS_WITH_BOOM
#define GAIA_ROCK_PILLAR_LANDS_WITH_BOOM 0
#endif

/* On Treasure Isle, Gale the Djinni answers, and the battle starts, only while
   the leader faces east (TALK_TRIGGER_FACING(FACING_EAST)). In Japanese Gale
   answers from any side (plain TRIGGER_TALK). */
#ifndef GALE_TALK_NEEDS_LEADER_FACING_EAST
#define GALE_TALK_NEEDS_LEADER_FACING_EAST 0
#endif

/* Garet's ledge-hanging script, used while he hangs by his grip during the
   Jupiter Lighthouse fight, sets his height to 12 pixels. Japanese adds 12 to
   whatever height he already had. */
#ifndef GARET_HANG_HEIGHT_ABSOLUTE
#define GARET_HANG_HEIGHT_ABSOLUTE 0
#endif

/* The give-item event command runs, the player drops a carried item to make
   room, and the gift goes to a member other than the leader. 'gave <item> to
   <member>' is then not shown at all. The item and member are still pushed as
   message arguments. */
#ifndef GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN
#define GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN 0
#endif

/* The player drops a carried item to make room for a gift that goes to a member
   other than the leader. The 'gave <item> to <member>' message now waits for a
   button press. Before, it also closed by itself once the treasure jingle had
   finished: zoom only, without FIELD_MESSAGE_WAITING_FOR_SOUND. */
#ifndef GIFT_TO_MEMBER_AFTER_DROP_WAITS_FOR_BUTTON
#define GIFT_TO_MEMBER_AFTER_DROP_WAITS_FOR_BUTTON 0
#endif

/* mnGlyphDrawArm no longer reserves an unused 64-byte local array (16 words) on
   its stack. Nothing reads it, so nothing on screen changes; only the compiled
   stack frame differs (sub sp, #100 in ja, #44 in the other editions). */
#ifndef GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED
#define GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED 0
#endif

/* The leader cannot push the actor that Halt holds still. The exception is
   actor 1, because the code tests against 1 where -1 looks meant. With nothing
   halted, nothing changes. */
#ifndef HALTED_ACTOR_CANNOT_BE_PUSHED
#define HALTED_ACTOR_CANNOT_BE_PUSHED 0
#endif

/* In Mars Lighthouse's aerie, Halt cast on the unseen talk spot of Ivan, Kyle,
   Jenna's mother or Jenna's father holds that person instead. Talking to them
   then gives their own line in halting speech, and the talk ends the hold and
   restores their update and colour. In Japanese, Halt there stops only the
   unseen spot, and talking to it gives a generic halted line. */
#ifndef HALT_ON_MARS_AERIE_SPEAKERS
#define HALT_ON_MARS_AERIE_SPEAKERS 0
#endif

/* In Super Lucky Dice's higher-or-lower guess, B cannot leave any hi-lo game.
   Japanese blocks B only in the first. In games 2 to 5, B goes back to throwing
   and keeps hiLoGame and the target (the bug noted in dice_game.c). */
#ifndef HI_LO_GAMES_CANNOT_BE_QUIT
#define HI_LO_GAMES_CANNOT_BE_QUIT 0
#endif

/* Every way out of the ice slide that starts from a cell's edge
   (scLeaderSlideOnIceFromEdge) now runs its cleanup: update cleared, the
   faces-motion steering flag set, acceleration 1/4, the standing animation.
   Three things change against Japanese. With no direction held at a cell's
   centre, the slide goes to that cleanup instead of returning without it (50).
   The move to the cell's centre before sliding waits at most 180 ticks and
   ends at once if the slide is stopped (81); before, it was
   fdActorWaitForMove's 600-tick wait, which ignores the stop. A slide stopped
   between cells ends at once (101) instead of settling back to the cell's
   centre and retrying. In Japanese a retry can leave the leader steered by the
   pad, walking at triple animation speed with acceleration 1/2. */
#ifndef ICE_SLIDE_ENDS_CLEANLY
#define ICE_SLIDE_ENDS_CLEANLY 0
#endif

/* Coming out of the Islet Cave halls onto the Sea of Time Islet, the islet's
   init no longer runs its own fade-in. The field's default opening plays
   instead, and with it the leader's climb-out arrival from the halls' stairs-up
   exit. A side effect is that the surf palette cycle is never started, so the
   surf stays still until one of George's rides starts it. In Japanese the islet
   fades in by itself, starts the surf and plays no climb-out. A return from
   battle (kept positions) runs the islet's own fade and the surf in every
   edition. */
#ifndef ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT
#define ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT 0
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

/* Cyclone still scatters Izumo's weeds after the Serpent is slain. Once
   FLAG_SERPENT_SLAIN is set, scGetTriggers switches to
   IzumoTriggersOnceTheSerpentIsSlain, and that table gets the nine Cyclone
   rows the earlier IzumoTriggers table already has in every edition. In
   Japanese, Cyclone stops working on Izumo's weeds after the Serpent. The two
   Antidotes and the Lucky Medal under them can then no longer be found if they
   were missed. */
#ifndef IZUMO_CYCLONE_WORKS_AFTER_SERPENT
#define IZUMO_CYCLONE_WORKS_AFTER_SERPENT 0
#endif

/* Felix's ladder climb down the opening in Izumo's stone ring into the Izumo
   Ruins plays the stairs-down sound (SE_STAIRS_DOWN) as he steps into the hole.
   Japanese plays no sound there. */
#ifndef IZUMO_RUINS_DESCENT_SOUND
#define IZUMO_RUINS_DESCENT_SOUND 0
#endif

/* Crossing between Kandorean Temple's two areas no longer shows 'Kandorean
   Temple' again. The two areas are the gate with Poi's hall, and the caves
   beyond them: the Mimic's room, the Chestbeaters' room and the stream cave.
   Each such entrance clears FLAG_SHOW_AREA_NAME in the room's start
   function. */
#ifndef KANDOREAN_TEMPLE_NAME_NOT_REPEATED
#define KANDOREAN_TEMPLE_NAME_NOT_REPEATED 0
#endif

/* When Mars Lighthouse's flames throw the leader back over ordinary ground, the
   knock-back ends as soon as another solid actor overlaps the point 8 pixels
   ahead. In Japanese the leader is carried through it. This runs on ordinary
   ground only: the sliding-on-ice branch of the knock-back gets no actor check.
   The ice-slide routines already had this test in every edition. */
#ifndef KNOCK_BACK_STOPS_AT_ACTORS
#define KNOCK_BACK_STOPS_AT_ACTORS 0
#endif

/* The entered-password buffer reserves 320 symbols instead of a Gold password's
   260. Nothing reads or writes past 260, so only the memory layout changes. The
   Broken Seal's file screen gets 60 more bytes of stack. In The Lost Age, the
   60 spare bytes that the Japanese edition keeps after the decoded password
   move into the buffer, and the total size of the region stays the same. */
#ifndef LARGER_PASSWORD_BUFFER
#define LARGER_PASSWORD_BUFFER 0
#endif

/* At the Lemuria dock, Loho's cannon stands on the ship's deck once the ship
   carries it (FLAG_SHIP_CARRIES_CANNON) and is moved out of sight before that.
   The Japanese edition places no cannon at the Lemuria dock. The cannon on the
   decks in other scenes, such as Atteka Inlet, shows in every edition. */
#ifndef LEMURIA_DOCK_CANNON_ON_DECK
#define LEMURIA_DOCK_CANNON_ON_DECK 0
#endif

/* Lemuria's spring takes the coin or Lucky Medal only after A confirms the toss
   (lemuria_spring.c:485). In Japanese it is taken as soon as it is chosen (442,
   452 in the Japanese branch of the 331 block). So B at the toss prompt loses
   it, and so does No to "You can't carry more items. Do you still want to
   play?" (the medal is already gone). Tolbi's spring in The Broken Seal takes
   it on confirmation in every edition. */
#ifndef LEMURIA_SPRING_PAYS_ON_CONFIRM
#define LEMURIA_SPRING_PAYS_ON_CONFIRM 0
#endif

/* On Air's Rock's whirlwind lift (scAirsRockInsideRiseOnLift, the leader's
   80-pixel ride up), the leader's priority flags (priFlg: draw priority taken
   from the cells underfoot, depth-sort and platform bits) are saved before the
   ride and restored after it. The ride flickers those flags and then sets them
   to ACTOR_PRIORITY_AUTOMATIC. In Japanese the leader keeps AUTOMATIC alone
   after the ride. */
#ifndef LIFT_RESTORES_LEADER_PRIORITY
#define LIFT_RESTORES_LEADER_PRIORITY 0
#endif

/* When a message is measured, each line's count of stretch points is always
   stored for the spacing pass. Japanese stores it only while the debug flag
   FLAG_SPREAD_LINES is set, so there every line but the last gets its gap from
   whatever the stack held. */
#ifndef LINE_SPACING_COUNTS_ALWAYS_STORED
#define LINE_SPACING_COUNTS_ALWAYS_STORED 0
#endif

/* Lonesome George's swim to his secret spot closes in the window shape instead
   of around Felix, and the arrival there opens in the same shape. Japanese
   starts the arrival's transition with the islet's backdrop fade still in the
   transition word. After the arrival the backdrop fade is put back for the next
   exit, so the ride out matches the ride back, which uses the shape in every
   edition. */
#ifndef LONESOME_GEORGE_RIDES_SHAPE_TRANSITION
#define LONESOME_GEORGE_RIDES_SHAPE_TRANSITION 0
#endif

/* In the battle item list, the refusal shown for an item that cannot be used
   now, is broken or has no effect is copied up to 52 halfwords instead of
   32. */
#ifndef LONG_BATTLE_ITEM_REFUSAL
#define LONG_BATTLE_ITEM_REFUSAL 0
#endif

/* The battle Status screen copies a Djinni's effect line, for a set or a
   recovering Djinni, up to 52 halfwords instead of 32. The 'ready to summon'
   and 'no Djinn' lines stay at 32. */
#ifndef LONG_STATUS_DJINN_HELP
#define LONG_STATUS_DJINN_HELP 0
#endif

/* The battle Status screen copies an item's description line up to 52 halfwords
   instead of 32. Its 'cannot be used in battle' and 'no items' lines stay at
   32. */
#ifndef LONG_STATUS_ITEM_HELP
#define LONG_STATUS_ITEM_HELP 0
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

/* On a Lucky Wheels jackpot, the title letters' fall-and-bounce is stepped a
   second time as each letter is drawn, on top of the step at the end of the
   tick. The letters therefore fall and bounce about twice as fast. The Spanish
   title's ninth letter, which the tick-end step does not reach, moves once a
   tick. */
#ifndef LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK
#define LUCKY_WHEELS_TITLE_MOVES_TWICE_A_TICK 0
#endif

/* Once the Gabomba message has been carried, Madra's innkeeper, talked to from
   any side but across the counter, asks his line about the dead fish on the
   shore as a yes/no question and gives one of two answers. In Japanese he only
   says the line, and the two answers in its catalogue are never shown. */
#ifndef MADRA_INNKEEPER_YES_NO_QUESTION
#define MADRA_INNKEEPER_YES_NO_QUESTION 0
#endif

/* When the nearest place changes, the map screen clears its name window at once
   and draws the new name 6 ticks later, using whichever place is nearest then.
   Japanese clears and draws in the same tick. */
#ifndef MAP_SCREEN_NAME_DELAYED_REDRAW
#define MAP_SCREEN_NAME_DELAYED_REDRAW 0
#endif

/* In the World Map strip that the map screen leaves out (z < 1832, x < 12000,
   where FLAG_LEADER_OFF_MAP_SCREEN is set), the leader's map screen point is
   his position shifted by (65, 1966). Japanese uses the fixed point (8872,
   3320) there. The party marker, the cursor's start and the fallback with no
   nearest place all follow this point. */
#ifndef MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER
#define MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER 0
#endif

/* A map screen row that scrolls in copies at most 31 tiles (the 240-pixel view
   plus one) and stops at the image's last column. Japanese always copies 32
   tiles and reads past the row's end at the right edge. */
#ifndef MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE
#define MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE 0
#endif

/* In Mars Lighthouse's Venus tower, the first hall has gaps at cells (38, 8),
   (43, 8) and (47, 8). A grey stone that stops in one of these gaps turns its
   cell into floor Felix can walk over; any stone can fill the first two, and
   only the first stone can fill (47, 8). The gaps are recomputed when a push or
   Move of the first stone ends, after a Move or Carry of any stone, and on
   entering the hall, so a gap opens again when its stone leaves. */
#ifndef MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR
#define MARS_LIGHTHOUSE_FILLED_GAPS_BECOME_FLOOR 0
#endif

/* In the first hall of Mars Lighthouse's Venus tower, Felix can walk from cell
   (38, 11) into a grey stone to push it along the ledge. The stone now moves
   only if no other actor stands in the cell beyond it; in practice only another
   stone can be there. The Japanese release always pushes, so one stone can be
   pushed onto another. */
#ifndef MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE
#define MARS_LIGHTHOUSE_LEDGE_PUSH_BLOCKED_BY_STONE 0
#endif

/* Using the Mars Star on the dragon head after the first time plays a short
   scene. The star is set, Felix faces north and a question mark shows over him,
   the star falls out, and he picks it up and turns south. There is no rumble,
   shake, red fade, sparks, map copy or thawed palette. The first, full use sets
   FLAG_MARS_STAR_SET. In Japanese the trigger has no guard, so every use
   replays the whole thaw. */
#ifndef MARS_STAR_REUSE_SHORT_SCENE
#define MARS_STAR_REUSE_SHORT_SCENE 0
#endif

/* The all-pages text measure sets its widest-line width to 0 before setting the
   sixteen page heights instead of after. The values are the same; only the
   order of the compiled stores differs. */
#ifndef MAX_WIDTH_CLEAR_FIRST
#define MAX_WIDTH_CLEAR_FIRST 0
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

/* Only the compiled code differs; the behaviour is the same in every edition.
   When a new game starts, the DMA wait after the first two clears
   (MainRoutineStateWork and the scene state) builds the DMA3 address again. The
   Japanese ROM reuses the copy still left in r3. In the source, the later
   editions write the two fills out directly, while Japanese goes through a
   FAKEMATCH inline wrapper, gmMainRoutineStateClearStates. */
#ifndef NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS
#define NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS 0
#endif

/* In a new game's data transfer, cancelling the password screen no longer
   returns to the transfer-method menu. The game still applies a password
   transfer of the blank, just-reset new-game state (no Djinn, default party,
   data-transferred flags set) and goes on to naming. */
#ifndef NEW_GAME_IGNORES_PASSWORD_CANCEL
#define NEW_GAME_IGNORES_PASSWORD_CANCEL 0
#endif

/* Starting a new game from the title (Felix's opening at Venus Lighthouse)
   first sets the restart point to the Venus Lighthouse floor-switch room,
   entered from the spiral pillars room
   (VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS). The Japanese edition does not
   set a restart point there. */
#ifndef NEW_GAME_SETS_RESTART_POINT
#define NEW_GAME_SETS_RESTART_POINT 0
#endif

/* When an item name that itself ends in a full stop (an abbreviated name) is
   followed by the message's own full stop, that second full stop is dropped, so
   the text never shows '..'. */
#ifndef NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME
#define NO_DOUBLE_FULL_STOP_AFTER_ITEM_NAME 0
#endif

/* Two narrow glyphs share one 16-pixel sprite cell only when their widths add
   up to 14 or less in outlined text (15 in plain text), because outlining
   widens each glyph by a pixel. Without this, an outlined pair could come to 17
   pixels and spill into the next row of the cell. */
#ifndef OUTLINED_GLYPH_PAIRS_FIT_CELL
#define OUTLINED_GLYPH_PAIRS_FIT_CELL 0
#endif

/* In battle, while the player chooses a reserve member to switch into a party
   place, A is ignored until the first preview has been drawn: the swapped-in
   member moved into the place and highlighted. Japanese accepts A on the
   screen's very first tick, before that preview, and confirms the swap with
   the first reserve member unseen. */
#ifndef PARTY_SWAP_WAITS_FOR_PREVIEW
#define PARTY_SWAP_WAITS_FOR_PREVIEW 0
#endif

/* When a password is read, each of Isaac, Garet, Ivan and Mia gets the
   experience that their decoded level needs, not just the level. In the
   Japanese edition they keep the new-game default experience whatever level the
   password gives. */
#ifndef PASSWORD_LEVEL_SETS_EXPERIENCE
#define PASSWORD_LEVEL_SETS_EXPERIENCE 0
#endif

/* The Pause menu's heading always sits 8 pixels lower, at y -16 instead of -24
   above its window, whether or not debug mode is on. */
#ifndef PAUSE_HEADING_LOWER
#define PAUSE_HEADING_LOWER 0
#endif

/* On the Lemurian ship, the pillar room's two frozen puddles are kept at sprite
   priority 2 instead of 1, so they draw behind priority-1 sprites. The
   six-jelly room's and the pipe room's puddles keep priority 1 in every
   edition. */
#ifndef PILLAR_PUDDLES_LOW_PRIORITY
#define PILLAR_PUDDLES_LOW_PRIORITY 0
#endif

/* Two pit falls get their own landing scene: down the Crystal Powder chest's
   pit in Ankohl Ruins (scAnkohlRuinsLandFromFall) and down the cobweb pit in
   Gaia Rock (scGaiaRockInsideRaiseDust). Felix drops in from 130 pixels and
   lands sitting in a ring of white dust. A thump shakes the ground, then he
   stands. In Japanese neither function exists and both falls use the generic
   falling arrival: a drop from 160 pixels, a door-close sound and a lean
   back. */
#ifndef PIT_FALL_DUST_LANDING
#define PIT_FALL_DUST_LANDING 0
#endif

/* A step onto a platform actor that is standing still 16 pixels below the
   player's feet skips the map collision check only when the step point and two
   points 2 pixels to either side of it are all on that actor's cell. The
   Japanese edition tests the step point alone. A failed test does not block the
   step: it falls through to the normal on-platform and map collision checks. */
#ifndef PLATFORM_STEP_TESTS_BOTH_SIDES
#define PLATFORM_STEP_TESTS_BOTH_SIDES 0
#endif

/* After the meeting with Poi in Kandorean Temple, the hall's own transition,
   around the leader, is put back. In Japanese the next screen change still uses
   the noise wipe that the meeting set for its own scenes. */
#ifndef POI_MEETING_RESTORES_HALL_TRANSITION
#define POI_MEETING_RESTORES_HALL_TRANSITION 0
#endif

/* The Psynergy menu's list waits one more tick after drawing the selected
   Psynergy's description in the info window. It happens only when a description
   is actually drawn: the slot is not empty and no kept message holds the info
   window. */
#ifndef PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION
#define PSYNERGY_LIST_WAITS_AFTER_DESCRIPTION 0
#endif

/* While the Psynergy menu is open, it suspends the field's tasks, saves BG char
   block 1 (8192 bytes), fills it with colour 3 and lends it to text as the
   second range of text tiles. On closing it takes the tiles back, restores the
   block and resumes the field's tasks. This is the Item screen's change, but it
   starts in German, so English lacks it. */
#ifndef PSYNERGY_MENU_EXTRA_TEXT_TILES
#define PSYNERGY_MENU_EXTRA_TEXT_TILES 0
#endif

/* The Status screen's Psynergy page draws its help line again after every
   refresh: on entry, on an L/R character switch, and when the cursor or page
   moves. Other editions draw it only once, before the page's loop. */
#ifndef PSYNERGY_PAGE_REDRAWS_HELP
#define PSYNERGY_PAGE_REDRAWS_HELP 0
#endif

/* The Psynergy test room drops the first of the innkeeper's two Mind Read rows,
   the "Any time you like" line. Mind Read on the innkeeper then gives his
   second line, MsgStupidBriggs, because the trigger lookup returns the first
   matching row. */
#ifndef PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE
#define PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE 0
#endif

/* Typed text never adds the per-line gap that would spread a line across the
   window, so lines stay unjustified. The gaps are still measured, only never
   applied. */
#ifndef RAGGED_LINES
#define RAGGED_LINES 0
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

/* In Rising Dragon, the Masamune's unleash, each strike after the first moves
   the target to 200 percent of its anchor's x instead of 300, so it is knocked
   back less far. A later strike happens only in the two-dragon variant. The
   first strike always uses 150. */
#ifndef RISING_DRAGON_SHORTER_KNOCKBACK
#define RISING_DRAGON_SHORTER_KNOCKBACK 0
#endif

/* Once Scoop digs Crystal up in Yampi Desert Cave, the ripple-running flag
   (FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING) is cleared. That turns off the Scoop
   trigger on the ripple sand, so she cannot be dug up, dropped and challenged
   again while the party stays in the room. Japanese leaves the flag set. */
#ifndef SCOOP_DIGS_UP_CRYSTAL_ONCE
#define SCOOP_DIGS_UP_CRYSTAL_ONCE 0
#endif

/* The settings screen, opened from the Pause menu, shows the Pause heading
   above its window instead of the Settings heading. */
#ifndef SETTINGS_SHOW_PAUSE_HEADING
#define SETTINGS_SHOW_PAUSE_HEADING 0
#endif

/* The shop test room's Djinn event gives the party all eighteen ordinary Djinn
   of every element, not the spares. Each one goes on standby to the member
   holding the fewest. The Japanese event instead gives and sets each character
   a run of The Lost Age's own Djinn of one element: Felix Echo to Meld, Jenna
   Cannon to Core, Sheba Breath to Wheeze, Piers Fog to Rime. */
#ifndef SHOP_TEST_GIVES_EVERY_DJINNI
#define SHOP_TEST_GIVES_EVERY_DJINNI 0
#endif

/* Link-battle opponents' names get their 'Enemy' label (MsgEnemyLabel) measured
   up to 8 letters instead of 5, with a space between the label and the name
   instead of none. This covers The Broken Seal's Battle Arena and The Lost
   Age's link lobby, plus each game's unused link-battle receiver. */
#ifndef SPACED_ENEMY_NAME_PREFIX
#define SPACED_ENEMY_NAME_PREFIX 0
#endif

/* The battle Status screen's Djinn page does not draw the number of turns a
   recovering Djinni still needs. The Djinni's name is still drawn in yellow,
   and the help line still gives the count. */
#ifndef STATUS_DJINN_TURNS_HIDDEN
#define STATUS_DJINN_TURNS_HIDDEN 0
#endif

/* Once Sunshine has gone to the anvil, the unseen marker at the far end of his
   bed is moved to (0, 0), so talking to the empty bed or Mind Reading it no
   longer gives his lines. This happens when forging starts and again when the
   party enters the inn while he forges. */
#ifndef SUNSHINE_BED_SILENT_WHILE_FORGING
#define SUNSHINE_BED_SILENT_WHILE_FORGING 0
#endif

/* Super Lucky Dice no longer clamps the first bet it draws to the party's
   coins. This is a code-only change that no player can see. The attendant
   starts the game only when the party has at least that bet:
   scContigoOfferSuperLuckyDice checks coins against average level times 10, the
   same rate as DICE_BET_PER_LEVEL. So the clamp could never fire. */
#ifndef SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP
#define SUPER_LUCKY_DICE_NO_FIRST_BET_CLAMP 0
#endif

/* In Taopo Swamp's lava rocks room, walking east from cell (9, 21) makes Felix
   jump two cells over the lava onto the chest rock, once it has been pushed in
   and sunk at (11, 21). The jump uses a new step trigger and the event
   scTaopoSwampJumpForward. */
#ifndef TAOPO_CHEST_ROCK_JUMP
#define TAOPO_CHEST_ROCK_JUMP 0
#endif

/* For its first two seconds the title screen neither shows its pulsing 'Press
   Start' prompt nor accepts A or Start, so a press in that window is lost. This
   also applies to the prompt-less title shown after Idejima, where the only
   effect is that A/Start cannot end the hold early. The Japanese edition shows
   the prompt and takes the keys from the first tick. */
#ifndef TITLE_PROMPT_AND_KEYS_AFTER_DELAY
#define TITLE_PROMPT_AND_KEYS_AFTER_DELAY 0
#endif

/* When Isaac's party joins, the coins carried over from The Broken Seal are
   added to the party's coins and the total is then capped at COIN_MAX
   (999,999). Without the flag the addition can push coins past the cap that
   every other coin change respects. */
#ifndef TRANSFERRED_COINS_CAPPED
#define TRANSFERRED_COINS_CAPPED 0
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

/* Setting off on the Trial Road also takes away the six items that raise a stat
   for good (Power Bread, Cookie, Apple, Hard Nut, Mint, Lucky Pepper), along
   with the unequipped gear it already took. Like that gear, they are given back
   at the end from the inventory snapshot. A new gmItemRaisesStat picks them
   out. */
#ifndef TRIAL_ROAD_TAKES_STAT_ITEMS
#define TRIAL_ROAD_TAKES_STAT_ITEMS 0
#endif

/* The six-byte table of the HP and PP labels' tile numbers is left out of the
   ROM. It sits after the party status's divider column offsets, and nothing
   reads it, so only the position of the data after it changes and nothing
   visible differs. In TLA it is WindowUnusedPartyColumnBytes; in TBS it is the
   bytes after WindowPartyColumnOffsets. */
#ifndef UNUSED_HP_PP_LABEL_TILES_DROPPED
#define UNUSED_HP_PP_LABEL_TILES_DROPPED 0
#endif

/* A file loaded through Update sets FLAG_KEEP_PARTY_POSITION, as Continue does,
   so play resumes where the party was, with its actors, tile copies and music
   restored. In the Japanese edition an updated file enters its scene fresh, at
   the scene's default spot. */
#ifndef UPDATE_KEEPS_PARTY_POSITION
#define UPDATE_KEEPS_PARTY_POSITION 0
#endif

/* On a map with camera boxes (areas the view may not show), a view move from
   Tremor or Grind shakes the screen, as on the World Map. Japanese instead
   opens the map's bounds and swings the view there. Gondowan Cliffs is the only
   map with camera boxes. */
#ifndef VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS
#define VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS 0
#endif

/* mnTextGetWideStringWidth is used only to centre a battle target's name under
   the cursor. With this feature it counts 10 pixels for every font code, the
   kana marks included, except the space, which still counts 4. A code above the
   font counts nothing. Without it, each glyph counts its own width, the kana
   marks count nothing and a kanji counts 10. */
#ifndef WIDE_STRING_FIXED_WIDTH
#define WIDE_STRING_FIXED_WIDTH 0
#endif

/* Yallam's curly-haired man (SPRITE_MAN_WITH_CURLY_HAIR) is placed 16 px
   further south, at z 248 instead of 232. He also wanders in shorter steps
   closer to home: MOVEMENT_TYPE_WANDER_NEAR (steps of 2 to 4 px, within 8 px of
   home) instead of MOVEMENT_TYPE_WANDER (steps of 4 to 12 px, within 16 px of
   home). */
#ifndef YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS
#define YALLAM_CURLY_HAIRED_MAN_WANDERS_LESS 0
#endif

/* Sometimes Yampi Desert Cave's Crystal room is set up again while the west
   pillar has not yet been pushed home to cell (10, 11), for example on a return
   from a battle partway through the puzzle. The room then blocks the cell the
   pillar actually stands on. Japanese always blocks the pillar's starting cell
   (8, 10), which leaves the cell under a partly pushed pillar open. */
#ifndef YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL
#define YAMPI_WEST_PILLAR_BLOCKS_ITS_OWN_CELL 0
#endif

/* Entering the Trainer's Whip chest room from the Yampi Desert Cave points
   Retreat at the World Map door the party last came in by (the Guardian Ring
   chest, Trainer's Whip chest or hidden Psynergy Stone room, from the
   ENTERED_BY flags). The Japanese edition leaves Retreat at the area-change
   default, which is the room's own doorway from the cave. */
#ifndef YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR
#define YAMPI_WHIP_CHEST_RETREAT_TO_LAST_DOOR 0
#endif

/* The villager who talks about Yepp's songs, in the house behind Yallam's
   south-east door (drawn with Sunshine's child's sprite), stands at z 140
   instead of 136, which is 4 pixels further south. */
#ifndef YEPP_SONGS_VILLAGER_FURTHER_SOUTH
#define YEPP_SONGS_VILLAGER_FURTHER_SOUTH 0
#endif

/* When a glyph sprite is drawn 0 pixels wide, the glyph drawer returns a width
   of 1, so a zero-width glyph is not mistaken for 'no window sprite free',
   which also returns 0. */
#ifndef ZERO_WIDTH_GLYPH_ADVANCES
#define ZERO_WIDTH_GLYPH_ADVANCES 0
#endif

#endif
