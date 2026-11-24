	.section .rodata
	.global Field_WorldMapPerspectiveTiles
Field_WorldMapPerspectiveTiles:
	.incbin "games/tbs/src/map/world/blocks.blocks"
	.global Field_DefaultGridCells
Field_DefaultGridCells:
	.incbin "games/tbs/src/map/default/cell_tiles.table.lz"
	.byte 0x01	@ PACKER: left in the buffer by Field_WorldMapPerspectiveTiles+0x39eb
	.global Field_DefaultTileAnimation
Field_DefaultTileAnimation:
	.incbin "games/tbs/src/map/default/tileset_anims.script.lz"
	.byte 0x6b, 0x4e, 0xad	@ PACKER: left in the buffer by Field_DefaultGridCells+0x151
	.global Field_DefaultCellTilesAttributes
Field_DefaultCellTilesAttributes:
	.incbin "games/tbs/src/map/default/collision.collision.lz"
	.byte 0xfa, 0x60	@ PACKER: left in the buffer by Field_DefaultGridCells+0x48e
