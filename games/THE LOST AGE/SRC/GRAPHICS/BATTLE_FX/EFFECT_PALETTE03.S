@ BattleFx_EffectPalette03: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette03
BattleFx_EffectPalette03:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE03.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE03.bitmap.lz"
