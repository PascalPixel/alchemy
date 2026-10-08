	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "graphics/battle_fx/vortex_00.gbapal"
	.incbin "graphics/battle_fx/vortex.parts.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x912
