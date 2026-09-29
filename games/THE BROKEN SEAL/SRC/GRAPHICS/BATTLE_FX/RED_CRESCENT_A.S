@ BattleFx_RedCrescentSheetA: the 64-colour palette of its first picture, then its 4 pictures
@ (listed in RED_CRESCENT_A.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A.parts.lz"
