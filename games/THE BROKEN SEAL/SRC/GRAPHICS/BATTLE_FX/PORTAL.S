@ BattleFx_PortalSheet: the 64-colour palette of its first picture, then its 9 pictures
@ (listed in PORTAL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "GRAPHICS/BATTLE_FX/PORTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PORTAL.parts.lz"
