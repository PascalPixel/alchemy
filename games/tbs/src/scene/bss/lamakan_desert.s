	.section .bss,"aw",%nobits
	.global gHeatGauge_VramSlot
gHeatGauge_VramSlot:
	.space 16
	.global gHeatGauge_Sprites
gHeatGauge_Sprites:
	.space 80
	.global gHeatGauge_TileBuffer
gHeatGauge_TileBuffer:
	.space 2304
	.global gHeatGauge_Held
gHeatGauge_Held:
	.space 2
