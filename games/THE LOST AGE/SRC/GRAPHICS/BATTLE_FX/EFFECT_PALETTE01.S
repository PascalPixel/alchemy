@ BattleFx_EffectPalette01: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette01
BattleFx_EffectPalette01:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE01.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE01.bitmap.lz"
