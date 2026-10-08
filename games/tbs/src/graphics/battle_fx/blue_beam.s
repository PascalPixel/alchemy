	.section .rodata
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "graphics/battle_fx/blue_beam_00.gbapal"
	.incbin "graphics/battle_fx/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x6b5
