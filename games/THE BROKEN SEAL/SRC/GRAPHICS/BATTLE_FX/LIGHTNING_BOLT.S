@ BattleFx_LightningBoltSheet: a 24x480 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LightningBoltSheet
BattleFx_LightningBoltSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_BOLT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHTNING_BOLT.bitmap.lz"
