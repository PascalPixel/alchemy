@ BattleFx_EarthWallSheet: the 64-colour palette of its first picture, then its 14 pictures
@ (listed in EARTH_WALL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_EarthWallSheet
BattleFx_EarthWallSheet:
	.incbin "GRAPHICS/BATTLE_FX/EARTH_WALL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EARTH_WALL.parts.lz"
