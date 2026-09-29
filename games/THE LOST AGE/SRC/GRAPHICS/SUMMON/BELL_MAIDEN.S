@ Summon_BellMaidenTiles: 8-bit sprite tiles in a column 1596 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_BellMaidenTiles
Summon_BellMaidenTiles:
	.incbin "GRAPHICS/SUMMON/BELL_MAIDEN.gbapal"
	.incbin "GRAPHICS/SUMMON/BELL_MAIDEN.bitmap.lz"
