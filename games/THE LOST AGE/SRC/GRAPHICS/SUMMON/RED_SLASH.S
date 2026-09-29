@ Summon_RedSlashTiles: 8-bit sprite tiles in a column 572 pixels tall, their 160-colour
@ palette and then the tiles as a packed 8-bit bitmap.
	.section .rodata
	.global Summon_RedSlashTiles
Summon_RedSlashTiles:
	.incbin "GRAPHICS/SUMMON/RED_SLASH.gbapal"
	.incbin "GRAPHICS/SUMMON/RED_SLASH.bitmap.lz"
