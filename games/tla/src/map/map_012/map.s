	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map012
Field_Map012:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map012, .LField_Map012_METATILES
	map_block Field_Map012, .LField_Map012_SHAPES
	map_block Field_Map012, .LField_Map012_GRID
	map_block Field_Map012, .LField_Map012_SHAPE_GRID
	map_block Field_Map012, .LField_Map012_ANIMATION
	map_block Field_Map012, .LField_Map012_BLEND
	map_block Field_Map012, .LField_Map012_SCENE
.LField_Map012_METATILES:
	.incbin "games/tla/src/map/map_012/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map012_SHAPES:
	.incbin "games/tla/src/map/map_012/shapes.table.lz"
	.balign 4, 0
.LField_Map012_GRID:
	.incbin "games/tla/src/map/map_012/grid.parts.lz"
	.balign 4, 0
.LField_Map012_SHAPE_GRID:
	.incbin "games/tla/src/map/map_012/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map012_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map012_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map012_SCENE:
	.incbin "games/tla/src/map/map_012/scene.parts.lz"
	.balign 4, 0
