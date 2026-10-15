	.section .rodata
	.global gColosso_GaugePalette
gColosso_GaugePalette:
	.incbin "games/tbs/src/graphics/colosso/gauge.gbapal"
	.global gColosso_GaugeGraphics
gColosso_GaugeGraphics:
	.incbin "games/tbs/src/graphics/colosso/gauge.4bpp.lz"
	.balign 4, 0
