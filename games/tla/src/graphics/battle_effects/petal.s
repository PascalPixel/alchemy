	.section .rodata
	.global BattleEffect_PetalSheet
BattleEffect_PetalSheet:
	.incbin "games/tla/src/graphics/battle_effects/petal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/petal.bitmap.lz"
	.byte 0x68	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x250b
