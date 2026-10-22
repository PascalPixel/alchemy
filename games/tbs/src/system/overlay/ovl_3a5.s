	.section .bss,"aw",%nobits
	.global gHeatGauge_VramSlot
gHeatGauge_VramSlot:
	.space 16
	.global gHeatGauge_Sprites
gHeatGauge_Sprites:
	.space 0x50
	.global gHeatGauge_TileBuffer
gHeatGauge_TileBuffer:
	.space 0x900
	.global gHeatGauge_Held
gHeatGauge_Held:
	.space 2
