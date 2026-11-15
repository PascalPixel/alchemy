	.global Field_SeaOfTimeWhirlpool
Field_SeaOfTimeWhirlpool:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_CELL_PICTURES
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_SHAPES
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_GRID
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_SHAPE_GRID
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_ANIMATION
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_BLEND
	map_block Field_SeaOfTimeWhirlpool, .LField_SeaOfTimeWhirlpool_SCENE
.LField_SeaOfTimeWhirlpool_CELL_PICTURES:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_SHAPES:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/shapes.table.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_GRID:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/map.grid.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_SHAPE_GRID:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/shape_grid.plane.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_ANIMATION:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/animation.script.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_BLEND:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/blend.script.lz"
	.balign 4, 0
.LField_SeaOfTimeWhirlpool_SCENE:
	.incbin "games/tla/src/map/sea_of_time_whirlpool/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SeaOfTimeWhirlpoolPalette
Tileset_SeaOfTimeWhirlpoolPalette:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_whirlpool/palette.gbapal.lz"
	.byte 0x04, 0x08, 0xf0	@ PACKER: left in the buffer by Field_SeaOfTime1+0xf1
	.global Tileset_SeaOfTimeWhirlpoolTilesA
Tileset_SeaOfTimeWhirlpoolTilesA:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_whirlpool/tiles_a.4bpp.mtf"
	.byte 0x24, 0x03, 0x8f	@ PACKER: left in the buffer by Field_SeaOfTime1+0x10dd
	.global Tileset_SeaOfTimeWhirlpoolTilesB
Tileset_SeaOfTimeWhirlpoolTilesB:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_whirlpool/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1ed7
	.global Tileset_SeaOfTimeWhirlpoolTilesC
Tileset_SeaOfTimeWhirlpoolTilesC:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_whirlpool/tiles_c.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Tileset_SeaOfTimeWhirlpoolTilesB+0xfb7
	.global Tileset_SeaOfTimeWhirlpoolTilesD
Tileset_SeaOfTimeWhirlpoolTilesD:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_whirlpool/tiles_d.4bpp.mtf"
	.byte 0x24, 0x40	@ PACKER: left in the buffer by Field_SeaOfTime1+0x1f46
