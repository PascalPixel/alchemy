@ BattleFx_ThinBoltSheet: a 16x256 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ThinBoltSheet
BattleFx_ThinBoltSheet:
	.incbin "GRAPHICS/BATTLE_FX/THIN_BOLT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THIN_BOLT.bitmap.lz"
