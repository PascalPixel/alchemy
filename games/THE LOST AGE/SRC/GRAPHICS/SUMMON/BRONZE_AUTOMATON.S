@ Summon_BronzeAutomatonTiles: its 224-colour palette, then 23 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_BronzeAutomatonTiles
Summon_BronzeAutomatonTiles:
	.incbin "GRAPHICS/SUMMON/BRONZE_AUTOMATON.gbapal"
	.incbin "GRAPHICS/SUMMON/BRONZE_AUTOMATON.parts.lz"
