@ BattleFx_CloudPuffSheet: a 32x256 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CloudPuffSheet
BattleFx_CloudPuffSheet:
	.incbin "GRAPHICS/BATTLE_FX/CLOUD_PUFF.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CLOUD_PUFF.bitmap.lz"
