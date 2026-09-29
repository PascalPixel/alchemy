@ BattleFx_StarDotSheet: the 64-colour palette of its first picture, then its 40 pictures
@ (listed in STAR_DOT.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT.parts.lz"
