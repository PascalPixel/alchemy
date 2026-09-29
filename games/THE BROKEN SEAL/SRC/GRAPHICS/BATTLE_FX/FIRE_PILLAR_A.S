@ BattleFx_FirePillarSheetA: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in FIRE_PILLAR_A.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_FirePillarSheetA
BattleFx_FirePillarSheetA:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_A_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_PILLAR_A.parts.lz"
