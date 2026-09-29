@ Summon_GoldenDragonHeadTiles: 8-bit sprite tiles in a column 1084 pixels tall, their 240-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_GoldenDragonHeadTiles
Summon_GoldenDragonHeadTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON_HEAD.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON_HEAD.bitmap.lz"
