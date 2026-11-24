	.global Field_SeaOfTimeSwirls
Field_SeaOfTimeSwirls:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_CELL_TILES
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_SHAPES
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_GRID
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_SHAPE_GRID
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_ANIMATION
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_BLEND
	map_block Field_SeaOfTimeSwirls, .LField_SeaOfTimeSwirls_SCENE
.LField_SeaOfTimeSwirls_CELL_TILES:
	.incbin "games/tla/src/map/sea_of_time_swirls/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_SHAPES:
	.incbin "games/tla/src/map/sea_of_time_swirls/shapes.table.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_GRID:
	.incbin "games/tla/src/map/sea_of_time_swirls/map.grid.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_SHAPE_GRID:
	.incbin "games/tla/src/map/sea_of_time_swirls/shape_grid.plane.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_ANIMATION:
	.incbin "games/tla/src/map/sea_of_time_swirls/animation.script.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_BLEND:
	.incbin "games/tla/src/map/sea_of_time_swirls/blend.script.lz"
	.balign 4, 0
.LField_SeaOfTimeSwirls_SCENE:
	.incbin "games/tla/src/map/sea_of_time_swirls/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SeaOfTimeSwirlsPalette
Tileset_SeaOfTimeSwirlsPalette:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_swirls/palette.gbapal.lz"
	.byte 0x04, 0x08, 0xf0	@ PACKER: left in the buffer by Field_SeaOfTime1+0xf1
	.global Tileset_SeaOfTimeSwirlsTilesA
Tileset_SeaOfTimeSwirlsTilesA:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_swirls/tiles_a.4bpp.mtf"
	.byte 0x24, 0x03, 0x8f	@ PACKER: left in the buffer by Field_SeaOfTime1+0x10dd
	.global Tileset_SeaOfTimeSwirlsTilesB
Tileset_SeaOfTimeSwirlsTilesB:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_swirls/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1ed7
	.global Tileset_SeaOfTimeSwirlsTilesC
Tileset_SeaOfTimeSwirlsTilesC:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_swirls/tiles_c.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Tileset_SeaOfTimeSwirlsTilesB+0xfb7
	.global Tileset_SeaOfTimeSwirlsTilesD
Tileset_SeaOfTimeSwirlsTilesD:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_swirls/tiles_d.4bpp.mtf"
	.byte 0x24, 0x40	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1f46
