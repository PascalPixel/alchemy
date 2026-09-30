@ Summon_GoldenScalesTiles: their 16-colour palette, then 95 4-bit tiles drawn
@ as the sprites listed in GOLDEN_SCALES.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_GoldenScalesTiles
Summon_GoldenScalesTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_SCALES_1.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_SCALES.parts.lz"
