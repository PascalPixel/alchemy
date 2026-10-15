	.section .rodata
	.global Summon_WingedKnightBTiles
Summon_WingedKnightBTiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_b.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_b.parts.lz"
	.byte 0x83	@ PACKER: left in the buffer by Summon_WingedKnightATiles+0x19ab
