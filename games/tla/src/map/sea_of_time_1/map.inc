	.global Field_SeaOfTime1
Field_SeaOfTime1:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_CELL_PICTURES
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_SHAPES
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_GRID
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_SHAPE_GRID
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_ANIMATION
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_BLEND
	map_block Field_SeaOfTime1, .LField_SeaOfTime1_SCENE
.LField_SeaOfTime1_CELL_PICTURES:
	.incbin "games/tla/src/map/sea_of_time_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_SeaOfTime1_SHAPES:
	.incbin "games/tla/src/map/sea_of_time_1/shapes.table.lz"
	.balign 4, 0
.LField_SeaOfTime1_GRID:
	.incbin "games/tla/src/map/sea_of_time_1/map.grid.lz"
	.balign 4, 0
.LField_SeaOfTime1_SHAPE_GRID:
	.incbin "games/tla/src/map/sea_of_time_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_SeaOfTime1_ANIMATION:
	.incbin "games/tla/src/map/sea_of_time_1/animation.script.lz"
	.balign 4, 0
.LField_SeaOfTime1_BLEND:
	.incbin "games/tla/src/map/sea_of_time_1/blend.script.lz"
	.balign 4, 0
.LField_SeaOfTime1_SCENE:
	.incbin "games/tla/src/map/sea_of_time_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SeaOfTime1Palette
Tileset_SeaOfTime1Palette:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_1/palette.gbapal.lz"
	.byte 0x04, 0x08, 0xf0	@ PACKER: left in the buffer by Field_SeaOfTime1+0xf1
	.global Tileset_SeaOfTime1TilesA
Tileset_SeaOfTime1TilesA:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_1/tiles_a.4bpp.mtf"
	.byte 0x24, 0x03, 0x8f	@ PACKER: left in the buffer by Field_SeaOfTime1+0x10dd
	.global Tileset_SeaOfTime1TilesB
Tileset_SeaOfTime1TilesB:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_1/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1ed7
	.global Tileset_SeaOfTime1TilesC
Tileset_SeaOfTime1TilesC:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_1/tiles_c.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Tileset_SeaOfTime1TilesB+0xfb7
	.global Tileset_SeaOfTime1TilesD
Tileset_SeaOfTime1TilesD:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_1/tiles_d.4bpp.mtf"
	.byte 0x24, 0x40	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1f46
