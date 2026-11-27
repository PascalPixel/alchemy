@ The edition for the assembler: EDITION_INTERNATIONAL is 1 in every edition
@ but the Japanese, and building more than one edition at once is an error.
@ The features the assembly asks for are set here for each edition as
@ edition.h turns them on for C: each is 1 or 0, tested with .if.
	.ifndef ALCHEMY_EDITION_INC
	.set ALCHEMY_EDITION_INC, 1
	.set EDITION_INTERNATIONAL, 0
	.set HALT_ON_CARTRIDGE_REMOVED, 0
	.set TITLE_GLOBALS_LEAD_SHORTER, 0
	.set FIELD_MENU_STOPS_REVEAL_SPARKLES, 0
	.set IWRAM_LAYOUT_SHIFT, 0
	.set IWRAM_GAP_AFTER_KEYS_DIRECTION, 0
	.set UNUSED_HP_PP_LABEL_TILES_DROPPED, 0
	.ifdef TBS_EDITION_JA
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 0
	.endif
	.ifdef TBS_EDITION_EN
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.endif
	.ifdef TBS_EDITION_DE
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.set HALT_ON_CARTRIDGE_REMOVED, 1
	.set TITLE_GLOBALS_LEAD_SHORTER, 1
	.set FIELD_MENU_STOPS_REVEAL_SPARKLES, 1
	.set IWRAM_LAYOUT_SHIFT, 1
	.set IWRAM_GAP_AFTER_KEYS_DIRECTION, 1
	.endif
	.ifdef TBS_EDITION_ES
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.set HALT_ON_CARTRIDGE_REMOVED, 1
	.set TITLE_GLOBALS_LEAD_SHORTER, 1
	.set FIELD_MENU_STOPS_REVEAL_SPARKLES, 1
	.endif
	.ifdef TBS_EDITION_FR
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.set HALT_ON_CARTRIDGE_REMOVED, 1
	.set TITLE_GLOBALS_LEAD_SHORTER, 1
	.set FIELD_MENU_STOPS_REVEAL_SPARKLES, 1
	.set UNUSED_HP_PP_LABEL_TILES_DROPPED, 1
	.endif
	.ifdef TBS_EDITION_IT
	.ifdef EDITION_SELECTED
	.error "multiple game editions selected"
	.endif
	.set EDITION_SELECTED, 1
	.set EDITION_INTERNATIONAL, 1
	.set HALT_ON_CARTRIDGE_REMOVED, 1
	.set TITLE_GLOBALS_LEAD_SHORTER, 1
	.set FIELD_MENU_STOPS_REVEAL_SPARKLES, 1
	.endif
	.endif
