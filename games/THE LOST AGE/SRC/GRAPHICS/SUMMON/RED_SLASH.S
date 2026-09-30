@ Summon_RedSlashTiles: its 160-colour palette, then 1 8-bit sprites of
@ 64x64 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_RedSlashTiles
Summon_RedSlashTiles:
	.incbin "GRAPHICS/SUMMON/RED_SLASH.gbapal"
	.incbin "GRAPHICS/SUMMON/RED_SLASH.parts.lz"
