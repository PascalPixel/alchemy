@ Summon_WingedKnightCTiles: its 256-colour palette, then 13 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_WingedKnightCTiles
Summon_WingedKnightCTiles:
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_C.gbapal"
	.incbin "GRAPHICS/SUMMON/WINGED_KNIGHT_C.parts.lz"
