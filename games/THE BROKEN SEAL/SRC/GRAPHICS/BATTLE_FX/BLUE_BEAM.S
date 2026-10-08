	.section .rodata
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BEAM.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x6b5
