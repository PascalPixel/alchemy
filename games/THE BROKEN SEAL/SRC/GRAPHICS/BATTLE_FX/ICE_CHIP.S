@ BattleFx_IceChipSheet: the 64-colour palette of its first picture, then its 11 pictures
@ (listed in ICE_CHIP.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP.parts.lz"
