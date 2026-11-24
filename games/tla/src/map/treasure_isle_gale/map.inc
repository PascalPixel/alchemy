	.global Field_TreasureIsleGale
Field_TreasureIsleGale:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_CELL_TILES
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_SHAPES
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_GRID
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_SHAPE_GRID
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_ANIMATION
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_BLEND
	map_block Field_TreasureIsleGale, .LField_TreasureIsleGale_SCENE
.LField_TreasureIsleGale_CELL_TILES:
	.incbin "games/tla/src/map/treasure_isle_gale/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TreasureIsleGale_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_gale/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsleGale_GRID:
	.incbin "games/tla/src/map/treasure_isle_gale/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsleGale_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_gale/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsleGale_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsleGale_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsleGale_SCENE:
	.incbin "games/tla/src/map/treasure_isle_gale/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTreasureIsleGale
Tileset_PaletteTreasureIsleGale:
	.incbin "games/tla/src/graphics/tileset/palettes/treasure_isle_gale.gbapal.lz"
	.byte 0x00, 0x29	@ PACKER: left in the buffer by Field_TreasureIsleGale+0x14e
