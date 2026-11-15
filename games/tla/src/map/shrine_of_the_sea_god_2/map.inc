	.global Field_ShrineOfTheSeaGod2
Field_ShrineOfTheSeaGod2:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_SHAPES
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_GRID
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_ANIMATION
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_BLEND
	map_block Field_ShrineOfTheSeaGod2, .LField_ShrineOfTheSeaGod2_SCENE
.LField_ShrineOfTheSeaGod2_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_2/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_2/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod2_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_2/scene.scene.lz"
	.balign 4, 0
