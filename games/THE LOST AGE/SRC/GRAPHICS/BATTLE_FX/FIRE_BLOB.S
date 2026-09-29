@ BattleFx_FireBlobSheet: a 48x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FireBlobSheet
BattleFx_FireBlobSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BLOB.bitmap.lz"
