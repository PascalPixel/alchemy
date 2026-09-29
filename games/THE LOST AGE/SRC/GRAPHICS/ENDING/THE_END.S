@ Ending_TheEndPicture: a 512x120 panorama: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Ending_TheEndPicture
Ending_TheEndPicture:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/ENDING/THE_END_JA.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END_JA.bitmap.d7"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/ENDING/THE_END_DE.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END_DE.bitmap.d7"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/ENDING/THE_END_ES.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END_ES.bitmap.d7"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/ENDING/THE_END_ES.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END_ES.bitmap.d7"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/ENDING/THE_END_IT.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END_IT.bitmap.d7"
	.else
	.incbin "GRAPHICS/ENDING/THE_END.gbapal"
	.incbin "GRAPHICS/ENDING/THE_END.bitmap.d7"
	.endif
	.endif
	.endif
	.endif
	.endif
