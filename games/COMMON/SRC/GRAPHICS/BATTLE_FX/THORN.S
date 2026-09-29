@ BattleFx_ThornSheet: the 64-colour palette of its first picture, then its 9 pictures
@ (listed in THORN.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "GRAPHICS/BATTLE_FX/THORN_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THORN.parts.lz"
