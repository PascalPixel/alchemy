	.global Field_TreasureIsle2
Field_TreasureIsle2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_CELL_PICTURES
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_SHAPES
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_GRID
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_SHAPE_GRID
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_ANIMATION
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_BLEND
	map_block Field_TreasureIsle2, .LField_TreasureIsle2_SCENE
.LField_TreasureIsle2_CELL_PICTURES:
	.incbin "games/tla/src/map/treasure_isle_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TreasureIsle2_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_2/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsle2_GRID:
	.incbin "games/tla/src/map/treasure_isle_2/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsle2_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsle2_ANIMATION:
	.incbin "games/tla/src/map/treasure_isle_2/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsle2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsle2_SCENE:
	.incbin "games/tla/src/map/treasure_isle_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TreasureIsle2Palette
Tileset_TreasureIsle2Palette:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_2/palette.gbapal.lz"
	.byte 0x17, 0x3c	@ PACKER: left in the buffer by Field_TreasureIsle2+0x152
	.global Tileset_TreasureIsle2TilesA
Tileset_TreasureIsle2TilesA:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_2/tiles_a.4bpp.mtf"
	.global Tileset_TreasureIsle2TilesB
Tileset_TreasureIsle2TilesB:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_2/tiles_b.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Tileset_TreasureIsle2TilesA+0x166b
	.global Tileset_TreasureIsle2TilesC
Tileset_TreasureIsle2TilesC:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_2/tiles_c.4bpp.mtf"
	.global Tileset_TreasureIsle2TilesD
Tileset_TreasureIsle2TilesD:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_2/tiles_d.4bpp.mtf"
	.byte 0x16, 0x41	@ PACKER: left in the buffer by Tileset_TreasureIsle2TilesC+0x776
