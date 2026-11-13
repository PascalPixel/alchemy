.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gLemuriaPalace_FirstMapVram
gLemuriaPalace_FirstMapVram:
	.space 0x00000004
	.global gLemuriaPalace_SecondMapShown
gLemuriaPalace_SecondMapShown:
	.space 0x0000000c
	.global gLemuriaPalace_SecondMapSprite
gLemuriaPalace_SecondMapSprite:
	.space 0x00000028
	.global gLemuriaPalace_SecondMapTile
gLemuriaPalace_SecondMapTile:
	.space 0x00000008
	.global gLemuriaPalace_FirstMapSprite
gLemuriaPalace_FirstMapSprite:
	.space 0x00000028
	.global gLemuriaPalace_FirstMapTile
gLemuriaPalace_FirstMapTile:
	.space 0x00000004
	.global gLemuriaPalace_MapMode
gLemuriaPalace_MapMode:
	.space 0x00000004
	.global gLemuriaPalace_FirstMapShown
gLemuriaPalace_FirstMapShown:
	.space 0x00000004
	.global gLemuriaPalace_SecondMapVram
gLemuriaPalace_SecondMapVram:
	.space 0x00000004
	.global gLemuriaPalace_MapTime
gLemuriaPalace_MapTime:
