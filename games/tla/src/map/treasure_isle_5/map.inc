	.global Field_TreasureIsle5
Field_TreasureIsle5:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_CELL_PICTURES
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_SHAPES
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_GRID
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_SHAPE_GRID
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_ANIMATION
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_BLEND
	map_block Field_TreasureIsle5, .LField_TreasureIsle5_SCENE
.LField_TreasureIsle5_CELL_PICTURES:
	.incbin "games/tla/src/map/treasure_isle_5/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TreasureIsle5_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_5/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsle5_GRID:
	.incbin "games/tla/src/map/treasure_isle_5/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsle5_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsle5_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsle5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsle5_SCENE:
	.incbin "games/tla/src/map/treasure_isle_5/scene.scene.lz"
	.balign 4, 0
