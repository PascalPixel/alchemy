	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "GRAPHICS/BATTLE_FX/VORTEX_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VORTEX.parts.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x912
