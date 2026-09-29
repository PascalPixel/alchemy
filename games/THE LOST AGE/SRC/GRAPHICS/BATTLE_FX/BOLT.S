@ BattleFx_BoltSheet: a 32x512 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BoltSheet
BattleFx_BoltSheet:
	.incbin "GRAPHICS/BATTLE_FX/BOLT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOLT.bitmap.lz"
