@ Summon_BellMaidenTiles: its 256-colour palette, then 12 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_BellMaidenTiles
Summon_BellMaidenTiles:
	.incbin "GRAPHICS/SUMMON/BELL_MAIDEN.gbapal"
	.incbin "GRAPHICS/SUMMON/BELL_MAIDEN.parts.lz"
