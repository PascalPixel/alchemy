@ BattleFx_ExplosionSheet: a 32x216 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ExplosionSheet
BattleFx_ExplosionSheet:
	.incbin "GRAPHICS/BATTLE_FX/EXPLOSION.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EXPLOSION.bitmap.lz"
