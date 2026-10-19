	.section .rodata
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "games/tla/src/graphics/battle_fx/portal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleFx_WispSheet+0x622
