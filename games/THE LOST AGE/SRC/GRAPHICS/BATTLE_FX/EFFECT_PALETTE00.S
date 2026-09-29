@ BattleFx_EffectPalette00: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette00
BattleFx_EffectPalette00:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE00.bitmap.lz"
