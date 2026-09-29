@ Summon_PurpleArmourTiles: 95 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_PurpleArmourTiles
Summon_PurpleArmourTiles:
	.incbin "GRAPHICS/SUMMON/PURPLE_ARMOUR.gbapal"
	.incbin "GRAPHICS/SUMMON/PURPLE_ARMOUR.4bpp.lz"
