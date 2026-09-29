@ BattleFx_VioletCometSheet: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in VIOLET_COMET.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_VioletCometSheet
BattleFx_VioletCometSheet:
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VIOLET_COMET.parts.lz"
