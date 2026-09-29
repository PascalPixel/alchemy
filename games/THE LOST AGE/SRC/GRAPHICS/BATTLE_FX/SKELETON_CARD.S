@ BattleFx_SkeletonCardSheet: a 32x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SkeletonCardSheet
BattleFx_SkeletonCardSheet:
	.incbin "GRAPHICS/BATTLE_FX/SKELETON_CARD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SKELETON_CARD.bitmap.lz"
