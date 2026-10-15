	.section .rodata
	.global Korosseo_GaugePalette
Korosseo_GaugePalette:
	.incbin "games/tbs/src/graphics/colosso/gauge.gbapal"
	.global Korosseo_GaugeGraphics
Korosseo_GaugeGraphics:
	.incbin "games/tbs/src/graphics/colosso/gauge.4bpp.lz"
	.balign 4, 0
