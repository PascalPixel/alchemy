@ BattleFx_BoulderPicture: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BoulderPicture
BattleFx_BoulderPicture:
	.incbin "GRAPHICS/BATTLE_FX/BOULDER_PICTURE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOULDER_PICTURE.bitmap.lz"
