@ Summon_GoldenDragonTiles: its 240-colour palette, then 31 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_GoldenDragonTiles
Summon_GoldenDragonTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON.parts.lz"
