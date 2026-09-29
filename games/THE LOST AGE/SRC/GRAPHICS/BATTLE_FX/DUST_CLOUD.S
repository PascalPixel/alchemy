@ BattleFx_DustCloudSheet: a 32x48 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_DustCloudSheet
BattleFx_DustCloudSheet:
	.incbin "GRAPHICS/BATTLE_FX/DUST_CLOUD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/DUST_CLOUD.bitmap.lz"
