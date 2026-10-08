	.section .rodata
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "GRAPHICS/BATTLE_FX/PORTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PORTAL.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleFx_WispSheet+0x622
