	.section .rodata
	.global Korosseo_GaugePalette
Korosseo_GaugePalette:
	.incbin "graphics/korosseo/gauge.gbapal"
	.global Korosseo_GaugeGraphics
Korosseo_GaugeGraphics:
	.incbin "graphics/korosseo/gauge.4bpp.lz"
	.balign 4, 0
