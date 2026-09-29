@ BattleFx_GreenPillarSheet: the 64-colour palette of its first picture, then its 10 pictures
@ (listed in GREEN_PILLAR.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_GreenPillarSheet
BattleFx_GreenPillarSheet:
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GREEN_PILLAR.parts.lz"
