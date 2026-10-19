	.section .rodata
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "games/tla/src/graphics/battle_fx/ice_block_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/ice_block.parts.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x1586
