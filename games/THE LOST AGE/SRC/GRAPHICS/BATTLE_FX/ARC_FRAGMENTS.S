@ BattleFx_ArcFragmentsSheet: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ArcFragmentsSheet
BattleFx_ArcFragmentsSheet:
	.incbin "GRAPHICS/BATTLE_FX/ARC_FRAGMENTS.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ARC_FRAGMENTS.bitmap.lz"
