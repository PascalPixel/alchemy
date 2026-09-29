@ BattleFx_SlashSheet: the 64-colour palette of its first picture, then its 13 pictures
@ (listed in SLASH.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SLASH.parts.lz"
