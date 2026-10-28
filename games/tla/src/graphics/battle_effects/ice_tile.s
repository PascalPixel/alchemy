	.section .rodata
	.global BattleEffect_IceTileSheet
BattleEffect_IceTileSheet:
	.incbin "games/tla/src/graphics/battle_effects/ice_tile_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/ice_tile.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x106a
