@ Summon_WingedKnightBTiles: its 256-colour palette, then 16 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_WingedKnightBTiles
Summon_WingedKnightBTiles:
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_B.gbapal"
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_B.parts.lz"
