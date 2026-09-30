@ Summon_LeafWarriorTiles: its 256-colour palette, then 11 8-bit sprites of
@ 32x32 and 480 zero bytes (ZERO_FILL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global Summon_LeafWarriorTiles
Summon_LeafWarriorTiles:
	.incbin "GRAPHICS/SUMMON/LEAF_WARRIOR.gbapal"
	.incbin "GRAPHICS/SUMMON/LEAF_WARRIOR.parts.lz"
