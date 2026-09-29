@ Summon_SeaSerpentTiles: 8-bit sprite tiles in a column 1084 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_SeaSerpentTiles
Summon_SeaSerpentTiles:
	.incbin "GRAPHICS/SUMMON/SEA_SERPENT.gbapal"
	.incbin "GRAPHICS/SUMMON/SEA_SERPENT.bitmap.lz"
