	.global Field_DailaFlooded
Field_DailaFlooded:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaFlooded, .LField_DailaFlooded_CELL_TILES
	map_block Field_DailaFlooded, .LField_DailaFlooded_SHAPES
	map_block Field_DailaFlooded, .LField_DailaFlooded_GRID
	map_block Field_DailaFlooded, .LField_DailaFlooded_SHAPE_GRID
	map_block Field_DailaFlooded, .LField_DailaFlooded_ANIMATION
	map_block Field_DailaFlooded
	map_block Field_DailaFlooded, .LField_DailaFlooded_SCENE
.LField_DailaFlooded_CELL_TILES:
	.incbin "games/tla/src/map/daila_flooded/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_DailaFlooded_SHAPES:
	.incbin "games/tla/src/map/daila_flooded/shapes.table.lz"
	.balign 4, 0
.LField_DailaFlooded_GRID:
	.incbin "games/tla/src/map/daila_flooded/map.grid.lz"
	.balign 4, 0
.LField_DailaFlooded_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_flooded/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaFlooded_ANIMATION:
	.incbin "games/tla/src/map/daila_flooded/animation.script.lz"
	.balign 4, 0
.LField_DailaFlooded_SCENE:
	.incbin "games/tla/src/map/daila_flooded/scene.scene.lz"
	.global Tileset_DailaFloodedPalette
Tileset_DailaFloodedPalette:
	.incbin "games/tla/src/graphics/tileset/daila_flooded/palette.gbapal.lz"
	.byte 0x81, 0xc2	@ PACKER: left in the buffer by Field_DailaFlooded+0x196
	.global Tileset_DailaFloodedTilesA
Tileset_DailaFloodedTilesA:
	.incbin "games/tla/src/graphics/tileset/daila_flooded/tiles_a.4bpp.mtf"
	.global Tileset_DailaFloodedTilesB
Tileset_DailaFloodedTilesB:
	.incbin "games/tla/src/graphics/tileset/daila_flooded/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Tileset_DailaFloodedTilesA+0x270b
	.global Tileset_DailaFloodedTilesC
Tileset_DailaFloodedTilesC:
	.incbin "games/tla/src/graphics/tileset/daila_flooded/tiles_c.4bpp.mtf"
	.byte 0xc1	@ PACKER: left in the buffer by Tileset_DailaFloodedTilesA+0x2943
	.global Tileset_DailaFloodedTilesD
Tileset_DailaFloodedTilesD:
	.incbin "games/tla/src/graphics/tileset/daila_flooded/tiles_d.4bpp.mtf"
	.byte 0x67, 0x14	@ PACKER: left in the buffer by Tileset_DailaFloodedTilesC+0x14d2
