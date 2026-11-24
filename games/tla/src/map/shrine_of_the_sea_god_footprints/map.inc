	.global Field_ShrineOfTheSeaGodFootprints
Field_ShrineOfTheSeaGodFootprints:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_CELL_TILES
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_SHAPES
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_GRID
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_ANIMATION
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_BLEND
	map_block Field_ShrineOfTheSeaGodFootprints, .LField_ShrineOfTheSeaGodFootprints_SCENE
.LField_ShrineOfTheSeaGodFootprints_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodFootprints_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/scene.scene.lz"
