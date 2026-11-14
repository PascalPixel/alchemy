	.section .rodata
	.balign 4
@ The title's PRESS START, one 32x8 sprite per piece.
	.global gTitle_PromptTiles
gTitle_PromptTiles:
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/title/prompt_de.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/title/prompt_es.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/title/prompt_fr.4bpp32x8.plz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/title/prompt_it.4bpp32x8.plz"
	.else
	.incbin "games/tbs/src/graphics/title/prompt.4bpp32x8.plz"
	.endif
	.endif
	.endif
	.endif
