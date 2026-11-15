	.global Field_ShrineOfTheSeaGod1
Field_ShrineOfTheSeaGod1:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_SHAPES
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_GRID
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_ANIMATION
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_BLEND
	map_block Field_ShrineOfTheSeaGod1, .LField_ShrineOfTheSeaGod1_SCENE
.LField_ShrineOfTheSeaGod1_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod1_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_1/scene.scene.lz"
	.balign 4, 0
