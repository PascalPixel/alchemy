	.section .rodata
	.global Summon_WingedKnightATiles
Summon_WingedKnightATiles:
	.incbin "games/tla/src/graphics/summon/winged_knight_a.gbapal"
	.incbin "games/tla/src/graphics/summon/winged_knight_a.parts.lz"
	.byte 0x45, 0xb9, 0x7f	@ PACKER: left in the buffer by Summon_RobedGoddessTiles+0x1efd
