@ Summon_BronzeAutomatonTiles: 8-bit sprite tiles in a column 3004 pixels tall, their 224-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_BronzeAutomatonTiles
Summon_BronzeAutomatonTiles:
	.incbin "GRAPHICS/SUMMON/BRONZE_AUTOMATON.gbapal"
	.incbin "GRAPHICS/SUMMON/BRONZE_AUTOMATON.bitmap.lz"
