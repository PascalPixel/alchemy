@ BattleFx_VioletLightningSheetA: a 72x192 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletLightningSheetA
BattleFx_VioletLightningSheetA:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_LIGHTNING_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_LIGHTNING_A.bitmap.lz"
