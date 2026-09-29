@ BattleFx_EffectPalette04: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette04
BattleFx_EffectPalette04:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE04.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE04.bitmap.lz"
