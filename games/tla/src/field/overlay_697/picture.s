@ A 16-colour palette and the 18-tile picture drawn with it; Spanish and
@ Italian each have their own.
	.section .rodata
	.global Data_020062b4
Data_020062b4:
	.incbin "games/tla/src/field/overlay_697/picture.gbapal"
	.global Data_020062d4
Data_020062d4:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/field/overlay_697/pictr_es.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/field/overlay_697/pictr_it.4bpp.lz"
	.else
	.incbin "games/tla/src/field/overlay_697/picture.4bpp.lz"
	.endif
	.endif
