@ Summon_RedCrescentTiles: its 32-colour palette, then 1 8-bit sprites of
@ 64x64 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_RedCrescentTiles
Summon_RedCrescentTiles:
	.incbin "GRAPHICS/SUMMON/RED_CRESCENT.gbapal"
	.incbin "GRAPHICS/SUMMON/RED_CRESCENT.parts.lz"
