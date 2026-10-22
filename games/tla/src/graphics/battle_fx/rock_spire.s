	.section .rodata
	.global BattleEffect_RockSpireSheet
BattleEffect_RockSpireSheet:
	.incbin "games/tla/src/graphics/battle_fx/rock_spire.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/rock_spire.bitmap.lz"
	.byte 0xa0, 0x20, 0x72	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetB+0x7a1
