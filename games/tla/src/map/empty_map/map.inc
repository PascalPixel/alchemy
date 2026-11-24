	.global Field_EmptyMap
Field_EmptyMap:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_EmptyMap, .LField_EmptyMap_CELL_TILES
	map_block Field_EmptyMap, .LField_EmptyMap_SHAPES
	map_block Field_EmptyMap, .LField_EmptyMap_GRID
	map_block Field_EmptyMap, .LField_EmptyMap_SHAPE_GRID
	map_block Field_EmptyMap
	map_block Field_EmptyMap
	map_block Field_EmptyMap, .LField_EmptyMap_SCENE
.LField_EmptyMap_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_EmptyMap_SHAPES:
	.incbin "games/tla/src/map/empty_map/shapes.table.lz"
	.balign 4, 0
.LField_EmptyMap_GRID:
	.incbin "games/tla/src/map/empty_map/map.grid.lz"
	.balign 4, 0
.LField_EmptyMap_SHAPE_GRID:
	.incbin "games/tla/src/map/empty_map/shape_grid.plane.lz"
	.balign 4, 0
.LField_EmptyMap_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data26D
Resource_Data26D:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_EmptyMap+0xb
	.global Resource_Data26E
Resource_Data26E:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0xff	@ PACKER: left in the buffer by Field_EmptyMap+0x14e
	.global Resource_Data26F
Resource_Data26F:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data26E+0x13f
	.global Resource_Data270
Resource_Data270:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data26F+0x13f
	.global Resource_Data271
Resource_Data271:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data270+0x13f
