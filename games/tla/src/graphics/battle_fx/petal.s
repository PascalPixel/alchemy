	.section .rodata
	.global BattleFx_PetalSheet
BattleFx_PetalSheet:
	.incbin "games/tla/src/graphics/battle_fx/petal.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/petal.bitmap.lz"
	.byte 0x68	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x250b
