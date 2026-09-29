@ Summon_LeafWarriorTiles: 8-bit sprite tiles in a column 1468 pixels tall, their 256-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_LeafWarriorTiles
Summon_LeafWarriorTiles:
	.incbin "GRAPHICS/SUMMON/LEAF_WARRIOR.gbapal"
	.incbin "GRAPHICS/SUMMON/LEAF_WARRIOR.bitmap.lz"
