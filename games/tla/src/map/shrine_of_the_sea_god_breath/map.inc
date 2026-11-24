	.global Field_ShrineOfTheSeaGodBreath
Field_ShrineOfTheSeaGodBreath:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_CELL_TILES
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_SHAPES
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_GRID
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_ANIMATION
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_BLEND
	map_block Field_ShrineOfTheSeaGodBreath, .LField_ShrineOfTheSeaGodBreath_SCENE
.LField_ShrineOfTheSeaGodBreath_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_breath/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_breath/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_breath/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_breath/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_crossroads/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodBreath_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_breath/scene.scene.lz"
	.balign 4, 0
