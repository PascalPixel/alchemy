@ BattleFx_BubbleSheet: a 24x86 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BubbleSheet
BattleFx_BubbleSheet:
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BUBBLE.bitmap.lz"
