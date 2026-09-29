@ BattleFx_GoldShellSheet: the 64-colour palette of its first picture, then its 28 pictures
@ (listed in GOLD_SHELL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL.parts.lz"
