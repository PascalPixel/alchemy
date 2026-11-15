	.global Field_Settlements9
Field_Settlements9:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements9, .LField_Settlements9_CELL_PICTURES
	map_block Field_Settlements9, .LField_Settlements9_SHAPES
	map_block Field_Settlements9, .LField_Settlements9_GRID
	map_block Field_Settlements9, .LField_Settlements9_SHAPE_GRID
	map_block Field_Settlements9, .LField_Settlements9_ANIMATION
	map_block Field_Settlements9, .LField_Settlements9_BLEND
	map_block Field_Settlements9, .LField_Settlements9_SCENE
.LField_Settlements9_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_9/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Settlements9_SHAPES:
	.incbin "games/tla/src/map/settlements_9/shapes.table.lz"
	.balign 4, 0
.LField_Settlements9_GRID:
	.incbin "games/tla/src/map/settlements_9/map.grid.lz"
	.balign 4, 0
.LField_Settlements9_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_9/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements9_ANIMATION:
	.incbin "games/tla/src/map/settlements_9/animation.script.lz"
	.balign 4, 0
.LField_Settlements9_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Settlements9_SCENE:
	.incbin "games/tla/src/map/settlements_9/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Settlements9Palette
Tileset_Settlements9Palette:
	.incbin "games/tla/src/graphics/tileset/settlements_9/palette.gbapal.lz"
	.byte 0xfe, 0x07	@ PACKER: left in the buffer by Field_Settlements9+0x17e
	.global Tileset_Settlements9TilesA
Tileset_Settlements9TilesA:
	.incbin "games/tla/src/graphics/tileset/settlements_9/tiles_a.4bpp.mtf"
	.global Tileset_Settlements9TilesB
Tileset_Settlements9TilesB:
	.incbin "games/tla/src/graphics/tileset/settlements_9/tiles_b.4bpp.mtf"
	.byte 0x9d, 0x69	@ PACKER: left in the buffer by Tileset_Settlements9TilesA+0x1e02
	.global Tileset_Settlements9TilesC
Tileset_Settlements9TilesC:
	.incbin "games/tla/src/graphics/tileset/settlements_9/tiles_c.4bpp.mtf"
	.byte 0x84	@ PACKER: left in the buffer by Tileset_Settlements9TilesB+0x1d5b
	.global Tileset_Settlements9TilesD
Tileset_Settlements9TilesD:
	.incbin "games/tla/src/graphics/tileset/settlements_9/tiles_d.4bpp.mtf"
