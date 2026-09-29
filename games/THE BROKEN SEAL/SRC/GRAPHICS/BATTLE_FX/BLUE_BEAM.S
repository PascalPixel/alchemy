@ BattleFx_BlueBeamSheet: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in BLUE_BEAM.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM.parts.lz"
