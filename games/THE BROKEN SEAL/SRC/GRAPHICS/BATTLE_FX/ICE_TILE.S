	.section .rodata
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x106a
