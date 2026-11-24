	.global Field_ShrineOfTheSeaGodStatue
Field_ShrineOfTheSeaGodStatue:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_CELL_TILES
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_SHAPES
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_GRID
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_ANIMATION
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_BLEND
	map_block Field_ShrineOfTheSeaGodStatue, .LField_ShrineOfTheSeaGodStatue_SCENE
.LField_ShrineOfTheSeaGodStatue_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_statue/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_statue/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_statue/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_statue/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodStatue_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_statue/scene.scene.lz"
	.balign 4, 0
