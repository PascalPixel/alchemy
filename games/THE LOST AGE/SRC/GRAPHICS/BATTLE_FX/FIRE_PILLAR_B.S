@ BattleFx_FirePillarSheetB: a 24x279 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_B.bitmap.lz"
