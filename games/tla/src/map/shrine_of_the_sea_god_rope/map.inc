	.global Field_ShrineOfTheSeaGodRope
Field_ShrineOfTheSeaGodRope:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_CELL_TILES
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_SHAPES
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_GRID
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_ANIMATION
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_BLEND
	map_block Field_ShrineOfTheSeaGodRope, .LField_ShrineOfTheSeaGodRope_SCENE
.LField_ShrineOfTheSeaGodRope_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRope_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/scene.scene.lz"
	.balign 4, 0
