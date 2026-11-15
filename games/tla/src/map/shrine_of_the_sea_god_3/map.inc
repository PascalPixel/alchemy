	.global Field_ShrineOfTheSeaGod3
Field_ShrineOfTheSeaGod3:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_SHAPES
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_GRID
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_ANIMATION
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_BLEND
	map_block Field_ShrineOfTheSeaGod3, .LField_ShrineOfTheSeaGod3_SCENE
.LField_ShrineOfTheSeaGod3_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod3_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_3/scene.scene.lz"
	.balign 4, 0
