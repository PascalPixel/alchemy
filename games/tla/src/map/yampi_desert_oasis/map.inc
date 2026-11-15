	.global Field_YampiDesertOasis
Field_YampiDesertOasis:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_CELL_PICTURES
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_SHAPES
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_GRID
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_SHAPE_GRID
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_ANIMATION
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_BLEND
	map_block Field_YampiDesertOasis, .LField_YampiDesertOasis_SCENE
.LField_YampiDesertOasis_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_oasis/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertOasis_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_oasis/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertOasis_GRID:
	.incbin "games/tla/src/map/yampi_desert_oasis/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertOasis_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_oasis/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertOasis_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_oasis/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertOasis_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertOasis_SCENE:
	.incbin "games/tla/src/map/yampi_desert_oasis/scene.scene.lz"
	.balign 4, 0
