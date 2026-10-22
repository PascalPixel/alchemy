	.section .rodata
	.global gHeatGauge_Palette
gHeatGauge_Palette:
	.incbin "games/tbs/src/graphics/lamakan/sand.gbapal"
	.global gHeatGauge_Tiles
gHeatGauge_Tiles:
	.incbin "games/tbs/src/graphics/lamakan/sand.4bpp.lz"
	.byte 0xff	@ PACKER: unused post-stream residue; HeatGauge_Start uses the decoded tiles; writer unknown
	.balign 4, 0
