	.global Field_AirsRock1
Field_AirsRock1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRock1, .LField_AirsRock1_CELL_PICTURES
	map_block Field_AirsRock1, .LField_AirsRock1_SHAPES
	map_block Field_AirsRock1, .LField_AirsRock1_GRID
	map_block Field_AirsRock1, .LField_AirsRock1_SHAPE_GRID
	map_block Field_AirsRock1, .LField_AirsRock1_ANIMATION
	map_block Field_AirsRock1, .LField_AirsRock1_BLEND
	map_block Field_AirsRock1, .LField_AirsRock1_SCENE
.LField_AirsRock1_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_AirsRock1_SHAPES:
	.incbin "games/tla/src/map/airs_rock_1/shapes.table.lz"
	.balign 4, 0
.LField_AirsRock1_GRID:
	.incbin "games/tla/src/map/airs_rock_1/map.grid.lz"
	.balign 4, 0
.LField_AirsRock1_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRock1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_AirsRock1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRock1_SCENE:
	.incbin "games/tla/src/map/airs_rock_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AirsRock1Palette
Tileset_AirsRock1Palette:
	.incbin "games/tla/src/graphics/tileset/airs_rock_1/palette.gbapal.lz"
	.byte 0x98, 0xb7, 0xe6	@ PACKER: left in the buffer by Field_AirsRock1+0x191
	.global Tileset_AirsRock1TilesA
Tileset_AirsRock1TilesA:
	.incbin "games/tla/src/graphics/tileset/airs_rock_1/tiles_a.4bpp.mtf"
	.global Tileset_AirsRock1TilesB
Tileset_AirsRock1TilesB:
	.incbin "games/tla/src/graphics/tileset/airs_rock_1/tiles_b.4bpp.mtf"
	.byte 0x48	@ PACKER: left in the buffer by Tileset_AirsRock1TilesA+0x269f
	.global Tileset_AirsRock1TilesC
Tileset_AirsRock1TilesC:
	.incbin "games/tla/src/graphics/tileset/airs_rock_1/tiles_c.4bpp.mtf"
	.byte 0xe7, 0xe3, 0x0d	@ PACKER: left in the buffer by Tileset_AirsRock1TilesB+0x2681
	.global Tileset_AirsRock1TilesD
Tileset_AirsRock1TilesD:
	.incbin "games/tla/src/graphics/tileset/airs_rock_1/tiles_d.4bpp.mtf"
	.byte 0x7b	@ PACKER: left in the buffer by Tileset_AirsRock1TilesC+0x333
