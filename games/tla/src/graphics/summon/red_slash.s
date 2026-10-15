	.section .rodata
	.global Summon_RedSlashTiles
Summon_RedSlashTiles:
	.incbin "games/tla/src/graphics/summon/red_slash.gbapal"
	.incbin "games/tla/src/graphics/summon/red_slash.parts.lz"
	.byte 0xb2, 0x6a, 0xeb	@ PACKER: left in the buffer by Summon_CrimsonBeastTiles+0x529
