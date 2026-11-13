.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gTreasureIsle_SavedBgColors
gTreasureIsle_SavedBgColors:
	.space 0x00000020
	.global gTreasureIsle_BgColors
gTreasureIsle_BgColors:
	.space 0x00000020
	.global gTreasureIsle_SavedObjColors
gTreasureIsle_SavedObjColors:
	.space 0x00000020
	.global gTreasureIsle_ObjColors
gTreasureIsle_ObjColors:
