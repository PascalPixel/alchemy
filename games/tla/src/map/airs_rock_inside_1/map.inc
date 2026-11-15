	.global Field_AirsRockInside1
Field_AirsRockInside1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_CELL_PICTURES
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_SHAPES
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_GRID
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_SHAPE_GRID
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_ANIMATION
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_BLEND
	map_block Field_AirsRockInside1, .LField_AirsRockInside1_SCENE
.LField_AirsRockInside1_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInside1_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_1/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInside1_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_1/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInside1_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInside1_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInside1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInside1_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AirsRockInside1Palette
Tileset_AirsRockInside1Palette:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_1/palette.gbapal.lz"
	.byte 0x94, 0x75	@ PACKER: left in the buffer by Field_AirsRockInside1+0x14a
	.global Tileset_AirsRockInside1TilesA
Tileset_AirsRockInside1TilesA:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_1/tiles_a.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Field_AirsRockInside1+0x1e83
	.global Tileset_AirsRockInside1TilesB
Tileset_AirsRockInside1TilesB:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_1/tiles_b.4bpp.mtf"
	.byte 0x7f, 0x04	@ PACKER: left in the buffer by Tileset_AirsRockInside1TilesA+0x15a6
	.global Tileset_AirsRockInside1TilesC
Tileset_AirsRockInside1TilesC:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_1/tiles_c.4bpp.mtf"
	.byte 0x04, 0x40, 0xd8	@ PACKER: left in the buffer by Field_AirsRockInside1+0x1fa9
	.global Tileset_AirsRockInside1TilesD
Tileset_AirsRockInside1TilesD:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_1/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xf4	@ PACKER: left in the buffer by Tileset_AirsRockInside1TilesC+0xf6e
