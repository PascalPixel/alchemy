@ BattleFx_EffectPalette02: a 1x1 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_EffectPalette02
BattleFx_EffectPalette02:
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE02.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EFFECT_PALETTE02.bitmap.lz"
