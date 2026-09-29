@ Summon_CrimsonBeastTiles: 8-bit sprite tiles in a column 1596 pixels tall, their 192-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_CrimsonBeastTiles
Summon_CrimsonBeastTiles:
	.incbin "GRAPHICS/SUMMON/CRIMSON_BEAST.gbapal"
	.incbin "GRAPHICS/SUMMON/CRIMSON_BEAST.bitmap.lz"
