.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gGaiaRockInside_SavedSusa
gGaiaRockInside_SavedSusa:
	.space 0x00000080
	.global gGaiaRockInside_SavedSusaX
gGaiaRockInside_SavedSusaX:
	.space 0x00000004
	.global gGaiaRockInside_SavedSusaZ
gGaiaRockInside_SavedSusaZ:
	.space 0x00000004
	.global gGaiaRockInside_SavedSusaFrame
gGaiaRockInside_SavedSusaFrame:
	.space 0x00000004
	.global gGaiaRockInside_SavedSusaAnimation
gGaiaRockInside_SavedSusaAnimation:
