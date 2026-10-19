	.section .rodata
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "games/tla/src/graphics/battle_fx/ice_tile_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/ice_tile.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x106a
