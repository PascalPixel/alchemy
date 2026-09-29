@ BattleFx_IceTileSheet: the 64-colour palette of its first picture, then its 7 pictures
@ (listed in ICE_TILE.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_TILE.parts.lz"
