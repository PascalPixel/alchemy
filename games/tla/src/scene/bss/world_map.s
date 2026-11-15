.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gWorldMap_HiddenMembers
gWorldMap_HiddenMembers:
	.space 0x00000020
	.global gWorldMap_PlacedActors
gWorldMap_PlacedActors:
	.space 0x00000200
	.global gWorldMapBlend
gWorldMapBlend:
	.space 0x00000004
	.global gGauge_Shown
gGauge_Shown:
	.space 0x00000002
	.global gGauge_Vram
gGauge_Vram:
	.space 0x00000002
	.global gGauge_Countdown
gGauge_Countdown:
	.space 0x00000004
	.global gGauge_Sprite
gGauge_Sprite:
	.space 0x0000000c
	.global gGauge_Tiles
gGauge_Tiles:
	.space 0x00000100
	.global gGauge_SourceTiles
gGauge_SourceTiles:
	.space 0x00000100
