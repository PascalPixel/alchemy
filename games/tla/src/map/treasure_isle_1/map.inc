	.global Field_TreasureIsle1
Field_TreasureIsle1:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_CELL_PICTURES
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_SHAPES
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_GRID
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_SHAPE_GRID
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_ANIMATION
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_BLEND
	map_block Field_TreasureIsle1, .LField_TreasureIsle1_SCENE
.LField_TreasureIsle1_CELL_PICTURES:
	.incbin "games/tla/src/map/treasure_isle_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_TreasureIsle1_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_1/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsle1_GRID:
	.incbin "games/tla/src/map/treasure_isle_1/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsle1_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsle1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsle1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsle1_SCENE:
	.incbin "games/tla/src/map/treasure_isle_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TreasureIsle1Palette
Tileset_TreasureIsle1Palette:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_1/palette.gbapal.lz"
	.byte 0x9e, 0x4d	@ PACKER: left in the buffer by Field_TreasureIsle1+0x142
	.global Tileset_TreasureIsle1TilesA
Tileset_TreasureIsle1TilesA:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_1/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_TreasureIsle1TilesB
Tileset_TreasureIsle1TilesB:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_1/tiles_b.4bpp.mtf"
	.global Tileset_TreasureIsle1TilesC
Tileset_TreasureIsle1TilesC:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_1/tiles_c.4bpp.mtf"
	.byte 0x06, 0x6b, 0x35	@ PACKER: left in the buffer by Tileset_TreasureIsle1TilesB+0xbe5
	.global Tileset_TreasureIsle1TilesD
Tileset_TreasureIsle1TilesD:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_1/tiles_d.4bpp.mtf"
	.byte 0x88, 0x5f	@ PACKER: left in the buffer by Tileset_TreasureIsle1TilesC+0x3fa
