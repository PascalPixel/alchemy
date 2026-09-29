@ BattleFx_BoltCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BoltCardSheet
BattleFx_BoltCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/BOLT_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOLT_CARD.bitmap.lz"
