@ Summon_WingedKnightCTiles: 8-bit sprite tiles in a column 1724 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_WingedKnightCTiles
Summon_WingedKnightCTiles:
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_C.gbapal"
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_C.bitmap.lz"
