@ Summon_ChainedDemonTiles: 8-bit sprite tiles in a column 3644 pixels tall, their 192-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_ChainedDemonTiles
Summon_ChainedDemonTiles:
	.incbin "GRAPHICS/SUMMON/CHAINED_DEMON.gbapal"
	.incbin "GRAPHICS/SUMMON/CHAINED_DEMON.bitmap.lz"
