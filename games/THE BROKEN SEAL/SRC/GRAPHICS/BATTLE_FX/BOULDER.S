@ BattleFx_BoulderSheet: the 64-colour palette of its first picture, then its 16 pictures
@ (listed in BOULDER.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BoulderSheet
BattleFx_BoulderSheet:
	.incbin "GRAPHICS/BATTLE_FX/BOULDER_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BOULDER.parts.lz"
