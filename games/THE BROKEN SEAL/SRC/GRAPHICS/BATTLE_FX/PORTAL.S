	.section .rodata
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "GRAPHICS/BATTLE_FX/PORTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PORTAL.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_WispSheet at +0x622 */
	.byte 0x3f, 0x10
