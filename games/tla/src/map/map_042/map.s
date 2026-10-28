	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map042
Field_Map042:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map042, .LField_Map042_CELL_PICTURES
	map_block Field_Map042, .LField_Map042_SHAPES
	map_block Field_Map042, .LField_Map042_GRID
	map_block Field_Map042, .LField_Map042_SHAPE_GRID
	map_block Field_Map042, .LField_Map042_ANIMATION
	map_block Field_Map042, .LField_Map042_BLEND
	map_block Field_Map042, .LField_Map042_SCENE
.LField_Map042_CELL_PICTURES:
	.incbin "games/tla/src/map/map_042/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map042_SHAPES:
	.incbin "games/tla/src/map/map_042/shapes.table.lz"
	.balign 4, 0
.LField_Map042_GRID:
	.incbin "games/tla/src/map/map_042/grid.parts.lz"
	.balign 4, 0
.LField_Map042_SHAPE_GRID:
	.incbin "games/tla/src/map/map_042/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map042_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map042_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map042_SCENE:
	.incbin "games/tla/src/map/map_042/scene.parts.lz"
	.balign 4, 0
