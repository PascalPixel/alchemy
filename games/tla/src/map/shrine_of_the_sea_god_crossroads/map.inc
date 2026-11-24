	.global Field_ShrineOfTheSeaGodCrossroads
Field_ShrineOfTheSeaGodCrossroads:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_CELL_TILES
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_SHAPES
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_GRID
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_ANIMATION
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_BLEND
	map_block Field_ShrineOfTheSeaGodCrossroads, .LField_ShrineOfTheSeaGodCrossroads_SCENE
.LField_ShrineOfTheSeaGodCrossroads_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodCrossroads_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/scene.scene.lz"
	.balign 4, 0
