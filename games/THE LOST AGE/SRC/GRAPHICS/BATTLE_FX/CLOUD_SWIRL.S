@ BattleFx_CloudSwirlSheet: a 128x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CloudSwirlSheet
BattleFx_CloudSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/CLOUD_SWIRL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CLOUD_SWIRL.bitmap.lz"
