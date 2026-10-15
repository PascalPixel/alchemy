	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "games/tla/src/graphics/battle_fx/vortex.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/vortex.bitmap.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x912
