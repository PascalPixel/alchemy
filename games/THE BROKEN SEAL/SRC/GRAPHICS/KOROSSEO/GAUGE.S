	.section .rodata
	.global Korosseo_GaugePalette
Korosseo_GaugePalette:
	.incbin "GRAPHICS/KOROSSEO/GAUGE.gbapal"
	.global Korosseo_GaugeGraphics
Korosseo_GaugeGraphics:
	.incbin "GRAPHICS/KOROSSEO/GAUGE.4bpp.lz"
	.balign 4, 0
