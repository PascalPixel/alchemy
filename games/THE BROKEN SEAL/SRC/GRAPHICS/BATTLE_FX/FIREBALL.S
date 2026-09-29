@ BattleFx_FireballSheet: a 40x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIREBALL.bitmap.lz"
