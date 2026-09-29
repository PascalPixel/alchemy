@ Summon_WhiteSpikesTiles: their 16-colour palette, then 111 4-bit tiles drawn
@ as the sprites listed in WHITE_SPIKES.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_WhiteSpikesTiles
Summon_WhiteSpikesTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_SPIKES_1.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_SPIKES.parts.lz"
