	.section .rodata
	.global BattleEffect_BlueBeamSheet
BattleEffect_BlueBeamSheet:
	.incbin "games/tla/src/graphics/battle_effects/blue_beam_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x6b5
