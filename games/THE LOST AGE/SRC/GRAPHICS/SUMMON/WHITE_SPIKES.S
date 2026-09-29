@ Summon_WhiteSpikesTiles: 111 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_WhiteSpikesTiles
Summon_WhiteSpikesTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_SPIKES.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_SPIKES.4bpp.lz"
