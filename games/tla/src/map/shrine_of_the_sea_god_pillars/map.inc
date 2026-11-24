	.global Field_ShrineOfTheSeaGodPillars
Field_ShrineOfTheSeaGodPillars:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_CELL_TILES
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_SHAPES
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_GRID
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_ANIMATION
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_BLEND
	map_block Field_ShrineOfTheSeaGodPillars, .LField_ShrineOfTheSeaGodPillars_SCENE
.LField_ShrineOfTheSeaGodPillars_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillars_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillars/scene.scene.lz"
	.balign 4, 0
