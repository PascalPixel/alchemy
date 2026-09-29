@ Summon_RedCrescentTiles: 8-bit sprite tiles in a column 572 pixels tall, their 32-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_RedCrescentTiles
Summon_RedCrescentTiles:
	.incbin "GRAPHICS/SUMMON/RED_CRESCENT.gbapal"
	.incbin "GRAPHICS/SUMMON/RED_CRESCENT.bitmap.lz"
