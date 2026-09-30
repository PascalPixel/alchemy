@ Summon_WhiteWispsTiles: their 16-colour palette, then 143 4-bit tiles drawn
@ as the sprites listed in WHITE_WISPS.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_WhiteWispsTiles
Summon_WhiteWispsTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_WISPS_1.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_WISPS.parts.lz"
