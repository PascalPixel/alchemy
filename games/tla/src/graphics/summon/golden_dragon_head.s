	.section .rodata
	.global Summon_GoldenDragonHeadTiles
Summon_GoldenDragonHeadTiles:
	.incbin "games/tla/src/graphics/summon/golden_dragon_head.gbapal"
	.incbin "games/tla/src/graphics/summon/golden_dragon_head.parts.lz"
	.byte 0x1e	@ PACKER: left in the buffer by Summon_BronzeAutomatonTiles+0x13df
