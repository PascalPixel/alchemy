@ BattleFx_DustBlobSheet: a 32x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DustBlobSheet
BattleFx_DustBlobSheet:
	.incbin "GRAPHICS/BATTLE_FX/DUST_BLOB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DUST_BLOB.bitmap.lz"
