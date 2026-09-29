@ BattleFx_SwordSlashSheet: the 64-colour palette of its first picture, then its 4 pictures
@ (listed in SWORD_SLASH.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SWORD_SLASH.parts.lz"
