@ BattleFx_FireBurstSheet: a 40x480 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FireBurstSheet
BattleFx_FireBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_BURST.bitmap.lz"
