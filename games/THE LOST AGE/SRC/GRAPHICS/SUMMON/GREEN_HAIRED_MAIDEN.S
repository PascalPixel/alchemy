@ Summon_GreenHairedMaidenTiles: its 240-colour palette, then 12 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_GreenHairedMaidenTiles
Summon_GreenHairedMaidenTiles:
	.incbin "GRAPHICS/SUMMON/GREEN_HAIRED_MAIDEN.gbapal"
	.incbin "GRAPHICS/SUMMON/GREEN_HAIRED_MAIDEN.parts.lz"
