@ BattleFx_BlueBeastSheet: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in BLUE_BEAST.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BlueBeastSheet
BattleFx_BlueBeastSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAST_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAST.parts.lz"
