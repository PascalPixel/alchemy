@ BattleFx_ThornVineSheet: the 64-colour palette of its first picture, then its 17 pictures
@ (listed in THORN_VINE.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/THORN_VINE.parts.lz"
