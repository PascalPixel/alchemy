	.section .rodata
	.global Summon_LeafWarriorTiles
Summon_LeafWarriorTiles:
	.incbin "games/tla/src/graphics/summon/leaf_warrior.gbapal"
	.incbin "games/tla/src/graphics/summon/leaf_warrior.parts.lz"
	.byte 0xe3, 0xf6, 0x14	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x166d
