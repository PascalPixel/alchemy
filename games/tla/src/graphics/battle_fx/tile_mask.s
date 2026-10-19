	.section .rodata
	.global BattleFx_TileAnimationMask
BattleFx_TileAnimationMask:
	.incbin "games/tla/src/graphics/battle_fx/tile_mask_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/tile_mask.parts.lz"
	.byte 0x38, 0xfa, 0x06	@ PACKER: left in the buffer by BattleFx_CloudPuffSheet+0x845
