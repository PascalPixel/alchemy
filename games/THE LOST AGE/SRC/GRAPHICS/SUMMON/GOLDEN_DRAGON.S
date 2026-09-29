@ Summon_GoldenDragonTiles: 8-bit sprite tiles in a column 4028 pixels tall, their 240-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_GoldenDragonTiles
Summon_GoldenDragonTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON.bitmap.lz"
