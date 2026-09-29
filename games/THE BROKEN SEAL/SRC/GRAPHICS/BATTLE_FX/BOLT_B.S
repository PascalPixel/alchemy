@ BattleFx_VioletLightningSheetB: a 72x216 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOLT_B.bitmap.lz"
