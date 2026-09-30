@ Summon_PurpleArmourTiles: their 16-colour palette, then 95 4-bit tiles drawn
@ as the sprites listed in PURPLE_ARMOUR.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_PurpleArmourTiles
Summon_PurpleArmourTiles:
	.incbin "GRAPHICS/SUMMON/PURPLE_ARMOUR_1.gbapal"
	.incbin "GRAPHICS/SUMMON/PURPLE_ARMOUR.parts.lz"
