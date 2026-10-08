	.section .rodata
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_WindStreakSheet at +0x6b5 */
	.byte 0x01, 0x53, 0x33
