@ A 16-colour palette and the 18-tile picture drawn with it; Spanish has its own.
	.section .rodata
	.global Data_020062b4
Data_020062b4:
	.incbin "field/overlay_697/picture.gbapal"
	.global Data_020062d4
Data_020062d4:
	.ifdef TLA_EDITION_ES
	.incbin "field/overlay_697/pictr_es.4bpp.lz"
	.else
	.incbin "field/overlay_697/picture.4bpp.lz"
	.endif
