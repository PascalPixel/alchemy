@ BattleFx_FlareSheet: the 64-colour palette of its first picture, then its 8 pictures
@ (listed in FLARE.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_FlareSheet
BattleFx_FlareSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLARE.parts.lz"
