	.section .rodata
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "games/tla/src/graphics/battle_fx/wisp.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x7ae
