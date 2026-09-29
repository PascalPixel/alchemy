@ Summon_WingedKnightBTiles: 8-bit sprite tiles in a column 2108 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_WingedKnightBTiles
Summon_WingedKnightBTiles:
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_B.gbapal"
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_B.bitmap.lz"
