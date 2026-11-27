@ The title's globals. They start on a halfword, right after its code, where a
@ C file holding its sprites' words would start them on a word.
	.include "games/tbs/include/edition.inc"
	.section .bss,"aw",%nobits
	.if TITLE_GLOBALS_LEAD_SHORTER
	.space 54
	.else
	.space 58
	.endif
	.global TitleRevealTick
TitleRevealTick:
	.space 20
	.global TitleSprites
TitleSprites:
	.ifdef TBS_EDITION_EN
	.space 216
	.else
	.ifeq EDITION_INTERNATIONAL
	.space 216
	.else
	.space 1440
	.endif
	.endif
