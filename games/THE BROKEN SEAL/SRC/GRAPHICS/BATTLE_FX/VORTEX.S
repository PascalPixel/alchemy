	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "GRAPHICS/BATTLE_FX/VORTEX_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VORTEX.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_WindStreakSheet at +0x912 */
	.byte 0xdd, 0x20
