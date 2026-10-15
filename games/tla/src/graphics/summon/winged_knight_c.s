	.section .rodata
	.global Summon_WingedKnightCTiles
Summon_WingedKnightCTiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_c.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_c.parts.lz"
	.byte 0xdb	@ PACKER: left in the buffer by Summon_WingedKnightBTiles+0x1577
