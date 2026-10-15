	.section .rodata
	.global Colosso_GaugePalette
Colosso_GaugePalette:
	.incbin "games/tbs/src/graphics/colosso/gauge.gbapal"
	.global Colosso_GaugeGraphics
Colosso_GaugeGraphics:
	.incbin "games/tbs/src/graphics/colosso/gauge.4bpp.lz"
	.balign 4, 0
