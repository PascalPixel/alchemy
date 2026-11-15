	.global Field_YampiDesertOasisCave
Field_YampiDesertOasisCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_CELL_PICTURES
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_SHAPES
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_GRID
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_SHAPE_GRID
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_ANIMATION
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_BLEND
	map_block Field_YampiDesertOasisCave, .LField_YampiDesertOasisCave_SCENE
.LField_YampiDesertOasisCave_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_oasis_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_oasis_cave/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_GRID:
	.incbin "games/tla/src/map/yampi_desert_oasis_cave/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_oasis_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertOasisCave_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
