	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map084
Field_Map084:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map084, .LField_Map084_METATILES
	map_block Field_Map084, .LField_Map084_SHAPES
	map_block Field_Map084, .LField_Map084_GRID
	map_block Field_Map084, .LField_Map084_SHAPE_GRID
	map_block Field_Map084, .LField_Map084_ANIMATION
	map_block Field_Map084, .LField_Map084_BLEND
	map_block Field_Map084, .LField_Map084_SCENE
.LField_Map084_METATILES:
	.incbin "map/map_084/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map084_SHAPES:
	.incbin "map/map_084/shapes.table.lz"
	.balign 4, 0
.LField_Map084_GRID:
	.incbin "map/map_084/grid.parts.lz"
	.balign 4, 0
.LField_Map084_SHAPE_GRID:
	.incbin "map/map_084/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map084_ANIMATION:
	.incbin "map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map084_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map084_SCENE:
	.incbin "map/map_084/scene.parts.lz"
