	.section .rodata
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "games/tla/src/graphics/battle_fx/skull.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/skull.bitmap.lz"
	.byte 0x6c, 0x09, 0x63	@ PACKER: left in the buffer by BattleFx_PortalSheet+0x421
