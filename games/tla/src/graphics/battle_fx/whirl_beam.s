	.section .rodata
	.global BattleFx_WhirlBeamSheet
BattleFx_WhirlBeamSheet:
	.incbin "games/tla/src/graphics/battle_fx/whirl_beam.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/whirl_beam.bitmap.lz"
	.byte 0x54, 0xaa, 0x14	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x2169
