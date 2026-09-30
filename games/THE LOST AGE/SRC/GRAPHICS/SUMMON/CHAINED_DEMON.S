@ Summon_ChainedDemonTiles: its 192-colour palette, then 28 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_ChainedDemonTiles
Summon_ChainedDemonTiles:
	.incbin "GRAPHICS/SUMMON/CHAINED_DEMON.gbapal"
	.incbin "GRAPHICS/SUMMON/CHAINED_DEMON.parts.lz"
