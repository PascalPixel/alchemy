	.global Field_YampiDesertSandPass
Field_YampiDesertSandPass:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_CELL_TILES
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_SHAPES
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_GRID
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_SHAPE_GRID
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_ANIMATION
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_BLEND
	map_block Field_YampiDesertSandPass, .LField_YampiDesertSandPass_SCENE
.LField_YampiDesertSandPass_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_sand_pass/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_GRID:
	.incbin "games/tla/src/map/yampi_desert_sand_pass/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_sand_pass/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertSandPass_SCENE:
	.incbin "games/tla/src/map/yampi_desert_sand_pass/scene.scene.lz"
