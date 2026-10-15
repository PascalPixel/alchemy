	.section .rodata
	.global Summon_SeaSerpentTiles
Summon_SeaSerpentTiles:
	.incbin "games/tla/src/graphics/summon/sea_serpent.gbapal"
	.incbin "games/tla/src/graphics/summon/sea_serpent.parts.lz"
	.byte 0x01	@ PACKER: left in the buffer by Summon_GreenHairedMaidenTiles+0x11ff
