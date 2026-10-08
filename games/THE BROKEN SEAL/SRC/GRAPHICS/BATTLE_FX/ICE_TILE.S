	.section .rodata
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0x106a */
	.byte 0x00, 0x01
