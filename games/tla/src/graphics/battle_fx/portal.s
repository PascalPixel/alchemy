	.section .rodata
	.global BattleEffect_PortalSheet
BattleEffect_PortalSheet:
	.incbin "games/tla/src/graphics/battle_fx/portal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x622
