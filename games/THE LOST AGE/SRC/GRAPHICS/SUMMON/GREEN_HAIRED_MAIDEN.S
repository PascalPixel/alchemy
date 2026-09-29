@ Summon_GreenHairedMaidenTiles: 8-bit sprite tiles in a column 1596 pixels tall, their 240-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_GreenHairedMaidenTiles
Summon_GreenHairedMaidenTiles:
	.incbin "GRAPHICS/SUMMON/GREEN_HAIRED_MAIDEN.gbapal"
	.incbin "GRAPHICS/SUMMON/GREEN_HAIRED_MAIDEN.bitmap.lz"
