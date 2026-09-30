@ Summon_CrimsonBeastTiles: its 192-colour palette, then 12 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_CrimsonBeastTiles
Summon_CrimsonBeastTiles:
	.incbin "GRAPHICS/SUMMON/CRIMSON_BEAST.gbapal"
	.incbin "GRAPHICS/SUMMON/CRIMSON_BEAST.parts.lz"
