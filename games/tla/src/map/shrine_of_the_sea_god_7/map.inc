	.global Field_ShrineOfTheSeaGod7
Field_ShrineOfTheSeaGod7:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_SHAPES
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_GRID
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_ANIMATION
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_BLEND
	map_block Field_ShrineOfTheSeaGod7, .LField_ShrineOfTheSeaGod7_SCENE
.LField_ShrineOfTheSeaGod7_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod7_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_7/scene.scene.lz"
	.balign 4, 0
