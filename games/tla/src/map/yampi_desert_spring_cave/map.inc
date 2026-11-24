	.global Field_YampiDesertSpringCave
Field_YampiDesertSpringCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_CELL_TILES
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_SHAPES
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_GRID
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_SHAPE_GRID
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_ANIMATION
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_BLEND
	map_block Field_YampiDesertSpringCave, .LField_YampiDesertSpringCave_SCENE
.LField_YampiDesertSpringCave_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_spring_cave/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_spring_cave/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_GRID:
	.incbin "games/tla/src/map/yampi_desert_spring_cave/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_spring_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertSpringCave_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
