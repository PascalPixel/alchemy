	.section .rodata
	.balign 4
@ The title's PRESS START, one 32x8 sprite per piece; both games show the
@ same lettering in each language.
	.ifdef TBS_EDITION_DE
	.set PROMPT_DE, 1
	.endif
	.ifdef TLA_EDITION_DE
	.set PROMPT_DE, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set PROMPT_ES, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set PROMPT_ES, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set PROMPT_FR, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set PROMPT_FR, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set PROMPT_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set PROMPT_IT, 1
	.endif
	.global Title_PromptTiles
Title_PromptTiles:
	.ifdef PROMPT_DE
	.incbin "games/common/src/graphics/title/prompt_de.4bpp32x8.plz"
	.else
	.ifdef PROMPT_ES
	.incbin "games/common/src/graphics/title/prompt_es.4bpp32x8.plz"
	.else
	.ifdef PROMPT_FR
	.incbin "games/common/src/graphics/title/prompt_fr.4bpp32x8.plz"
	.else
	.ifdef PROMPT_IT
	.incbin "games/common/src/graphics/title/prompt_it.4bpp32x8.plz"
	.else
	.incbin "games/common/src/graphics/title/prompt.4bpp32x8.plz"
	.endif
	.endif
	.endif
	.endif
