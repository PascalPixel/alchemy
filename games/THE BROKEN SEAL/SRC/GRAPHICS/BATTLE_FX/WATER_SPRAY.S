@ BattleFx_WaterSpraySheet: the 64-colour palette of its first picture, then its 8 pictures
@ (listed in WATER_SPRAY.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_WaterSpraySheet
BattleFx_WaterSpraySheet:
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPRAY_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPRAY.parts.lz"
