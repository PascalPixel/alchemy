@ Summon_GoldenDragonHeadTiles: its 240-colour palette, then 8 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_GoldenDragonHeadTiles
Summon_GoldenDragonHeadTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON_HEAD.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_DRAGON_HEAD.parts.lz"
