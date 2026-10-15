	.section .rodata
	.global Summon_BellMaidenTiles
Summon_BellMaidenTiles:
	.incbin "games/tla/src/graphics/summon/bell_maiden.gbapal"
	.incbin "games/tla/src/graphics/summon/bell_maiden.parts.lz"
	.byte 0x4e, 0x39	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x1d22
