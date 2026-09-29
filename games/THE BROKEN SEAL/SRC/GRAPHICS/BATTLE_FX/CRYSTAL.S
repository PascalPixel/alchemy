@ BattleFx_CrystalSheet: the 64-colour palette of its first picture, then its 13 pictures
@ (listed in CRYSTAL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL.parts.lz"
