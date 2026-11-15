	.global Field_SeaOfTime2
Field_SeaOfTime2:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_CELL_PICTURES
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_SHAPES
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_GRID
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_SHAPE_GRID
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_ANIMATION
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_BLEND
	map_block Field_SeaOfTime2, .LField_SeaOfTime2_SCENE
.LField_SeaOfTime2_CELL_PICTURES:
	.incbin "games/tla/src/map/sea_of_time_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_SeaOfTime2_SHAPES:
	.incbin "games/tla/src/map/sea_of_time_2/shapes.table.lz"
	.balign 4, 0
.LField_SeaOfTime2_GRID:
	.incbin "games/tla/src/map/sea_of_time_2/map.grid.lz"
	.balign 4, 0
.LField_SeaOfTime2_SHAPE_GRID:
	.incbin "games/tla/src/map/sea_of_time_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_SeaOfTime2_ANIMATION:
	.incbin "games/tla/src/map/sea_of_time_1/animation.script.lz"
	.balign 4, 0
.LField_SeaOfTime2_BLEND:
	.incbin "games/tla/src/map/sea_of_time_1/blend.script.lz"
	.balign 4, 0
.LField_SeaOfTime2_SCENE:
	.incbin "games/tla/src/map/sea_of_time_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SeaOfTime2Palette
Tileset_SeaOfTime2Palette:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_2/palette.gbapal.lz"
	.byte 0x48, 0x1d, 0x3f	@ PACKER: left in the buffer by Field_SeaOfTime2+0x149
	.global Tileset_SeaOfTime2TilesA
Tileset_SeaOfTime2TilesA:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_2/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x30	@ PACKER: left in the buffer by Tileset_ChampaCliffCaves1TilesB+0x20bb
	.else
	.byte 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_SeaOfTime2TilesB
Tileset_SeaOfTime2TilesB:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_2/tiles_b.4bpp.mtf"
	.global Tileset_SeaOfTime2TilesC
Tileset_SeaOfTime2TilesC:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_2/tiles_c.4bpp.mtf"
	.byte 0xd7, 0x23	@ PACKER: left in the buffer by Tileset_SeaOfTime2TilesB+0x15c2
	.global Tileset_SeaOfTime2TilesD
Tileset_SeaOfTime2TilesD:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_2/tiles_d.4bpp.mtf"
