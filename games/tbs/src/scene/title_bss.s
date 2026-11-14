@ The title's globals. They start on a halfword, right after its code, where a
@ C file holding its sprites' words would start them on a word.
	.include "games/tbs/include/edition.inc"
	.section .bss,"aw",%nobits
	.ifdef TBS_EDITION_EN
	.space 58
	.else
	.ifeq EDITION_INTERNATIONAL
	.space 58
	.else
	.space 54
	.endif
	.endif
	.global gTitleRevealFrame
gTitleRevealFrame:
	.space 20
	.global gTitleSprites
gTitleSprites:
	.ifdef TBS_EDITION_EN
	.space 216
	.else
	.ifeq EDITION_INTERNATIONAL
	.space 216
	.else
	.space 1440
	.endif
	.endif
