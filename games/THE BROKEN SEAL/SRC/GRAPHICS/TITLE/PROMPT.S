	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
@ 「PRESS START」の文字 (32x8 のスプライトごと)
	.global Title_PromptTiles
Title_PromptTiles:
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/TITLE/PROMPT_DE.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TITLE/PROMPT_ES.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TITLE/PROMPT_FR.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TITLE/PROMPT_IT.4bpp32x8.plz"
	.else
	.incbin "GRAPHICS/TITLE/PROMPT.4bpp32x8.plz"
	.endif
	.endif
	.endif
	.endif
