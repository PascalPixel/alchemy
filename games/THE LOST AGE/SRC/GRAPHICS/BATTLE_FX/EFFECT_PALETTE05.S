@ BattleFx_EffectPalette05: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette05
BattleFx_EffectPalette05:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE05.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE05.bitmap.lz"
