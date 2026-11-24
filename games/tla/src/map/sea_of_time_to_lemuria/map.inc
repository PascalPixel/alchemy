	.global Field_SeaOfTimeToLemuria
Field_SeaOfTimeToLemuria:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_CELL_TILES
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_SHAPES
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_GRID
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_SHAPE_GRID
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_ANIMATION
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_BLEND
	map_block Field_SeaOfTimeToLemuria, .LField_SeaOfTimeToLemuria_SCENE
.LField_SeaOfTimeToLemuria_CELL_TILES:
	.incbin "games/tla/src/map/sea_of_time_to_lemuria/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_SHAPES:
	.incbin "games/tla/src/map/sea_of_time_to_lemuria/shapes.table.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_GRID:
	.incbin "games/tla/src/map/sea_of_time_to_lemuria/map.grid.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_SHAPE_GRID:
	.incbin "games/tla/src/map/sea_of_time_to_lemuria/shape_grid.plane.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_ANIMATION:
	.incbin "games/tla/src/map/sea_of_time_swirls/animation.script.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_BLEND:
	.incbin "games/tla/src/map/sea_of_time_swirls/blend.script.lz"
	.balign 4, 0
.LField_SeaOfTimeToLemuria_SCENE:
	.incbin "games/tla/src/map/sea_of_time_to_lemuria/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SeaOfTimeToLemuriaPalette
Tileset_SeaOfTimeToLemuriaPalette:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_to_lemuria/palette.gbapal.lz"
	.byte 0x48, 0x1d, 0x3f	@ PACKER: left in the buffer by Field_SeaOfTime2+0x149
	.global Tileset_SeaOfTimeToLemuriaTilesA
Tileset_SeaOfTimeToLemuriaTilesA:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_to_lemuria/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x30	@ PACKER: left in the buffer by Tileset_ChampaCliffCavesBriggsTilesB+0x20bb
	.else
	.byte 0x40	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_SeaOfTimeToLemuriaTilesB
Tileset_SeaOfTimeToLemuriaTilesB:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_to_lemuria/tiles_b.4bpp.mtf"
	.global Tileset_SeaOfTimeToLemuriaTilesC
Tileset_SeaOfTimeToLemuriaTilesC:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_to_lemuria/tiles_c.4bpp.mtf"
	.byte 0xd7, 0x23	@ PACKER: left in the buffer by Tileset_SeaOfTimeToLemuriaTilesB+0x15c2
	.global Tileset_SeaOfTimeToLemuriaTilesD
Tileset_SeaOfTimeToLemuriaTilesD:
	.incbin "games/tla/src/graphics/tileset/sea_of_time_to_lemuria/tiles_d.4bpp.mtf"
