@ BattleFx_VineSheet: the 64-colour palette of its first picture, then its 10 pictures
@ (listed in VINE.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "GRAPHICS/BATTLE_FX/VINE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VINE.parts.lz"
