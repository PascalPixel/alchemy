@ BattleFx_TileAnimationMask: the 64-colour palette of its first picture, then its 33 pictures
@ (listed in TILE_MASK.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_TileAnimationMask
BattleFx_TileAnimationMask:
	.incbin "GRAPHICS/BATTLE_FX/TILE_MASK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TILE_MASK.parts.lz"
