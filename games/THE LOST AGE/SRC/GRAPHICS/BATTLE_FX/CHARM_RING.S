@ BattleFx_CharmRingSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CharmRingSheet
BattleFx_CharmRingSheet:
	.incbin "GRAPHICS/BATTLE_FX/CHARM_RING.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CHARM_RING.bitmap.lz"
