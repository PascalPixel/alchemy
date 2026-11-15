	.global Field_ShrineOfTheSeaGod4
Field_ShrineOfTheSeaGod4:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_SHAPES
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_GRID
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_ANIMATION
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_BLEND
	map_block Field_ShrineOfTheSeaGod4, .LField_ShrineOfTheSeaGod4_SCENE
.LField_ShrineOfTheSeaGod4_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod4_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/scene.scene.lz"
