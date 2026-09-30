@ Summon_SeaSerpentTiles: its 256-colour palette, then 8 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_SeaSerpentTiles
Summon_SeaSerpentTiles:
	.incbin "GRAPHICS/SUMMON/SEA_SERPENT.gbapal"
	.incbin "GRAPHICS/SUMMON/SEA_SERPENT.parts.lz"
